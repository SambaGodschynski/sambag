/*
 * MenuSelectionManager.cpp
 *
 *  Created on: Thu Jun 14 13:23:23 2012
 *      Author: Johannes Unger
 */

#include "MenuSelectionManager.hpp"


namespace sambag { namespace disco { namespace components {
//=============================================================================
//  Class MenuSelectionManager
//=============================================================================
//-----------------------------------------------------------------------------.
MenuSelectionManager & MenuSelectionManager::defaultManager() {
	return Loki::SingletonHolder<MenuSelectionManager>::Instance();
}
//-----------------------------------------------------------------------------
void MenuSelectionManager::setSelectedPath(const
MenuSelectionManager:: MenuElements &path)
{
	size_t i, c;
	size_t currentSelectionCount = selection.size();
	size_t firstDifference = 0;

	for (i = 0, c = path.size(); i < c; i++) {
		if (i < currentSelectionCount && selection.at(i) == path[i])
			firstDifference++;
		else
			break;
	}

	for (i = currentSelectionCount - 1; i >= firstDifference; i--) {
		IMenuElement::Ptr me = selection.at(i);
		selection.erase(selection.begin() + i);
		me->menuSelectionChanged(false);
	}

	for (i = firstDifference, c = path.size(); i < c; i++) {
		if (path[i]) {
			selection.push_back(path[i]);
			path[i]->menuSelectionChanged(true);
		}
	}
	fireStateChanged();
}
//-----------------------------------------------------------------------------
void MenuSelectionManager::clearSelectedPath() {
	if (selection.empty())
		return;
	MenuElements tmp;
	setSelectedPath(tmp);
}
//-----------------------------------------------------------------------------
AComponentPtr MenuSelectionManager::componentForPoint(AComponentPtr source,
		const Point2D sourcePoint) const
{
	return AComponentPtr();
}
//-----------------------------------------------------------------------------
void MenuSelectionManager::fireStateChanged() {

}
//-----------------------------------------------------------------------------
bool
MenuSelectionManager::isComponentPartOfCurrentMenu(AComponentPtr c) const
{
	return false;
}
//-----------------------------------------------------------------------------
void MenuSelectionManager::processMouseEvent(const events::MouseEvent &ev)
{
	using namespace events;
	Coordinate screenX, screenY;
	Point2D p;
	int i, j, d;
	AComponentPtr mc;
	Rectangle r2;
	Coordinate cWidth, cHeight;
	IMenuElement::Ptr menuElement;
	MenuElements subElements;
	MenuElements path;
	MenuElements tmp;
	int selectionSize;
	p = ev.getLocation();

	AComponentPtr source = ev.getSource();

	if (source && !source->isShowing()) {
		return;
	}

	int type = ev.getType();
	if ((type == MouseEvent::MOUSE_ENTERED || type == MouseEvent::MOUSE_EXITED)
			&& ev.getButtons() > 0)
	{
		return;
	}

	if (source) {
		//SwingUtilities.convertPointToScreen(p, source);
		p = source->getLocationOnScreen(p);
	}

	screenX = p.x();
	screenY = p.y();

	tmp = selection;
	selectionSize = tmp.size();
	bool success = false;
	for (i = selectionSize - 1; i >= 0 && success == false; i--) {
		menuElement = tmp.at(i);
		subElements.clear();
		menuElement->getSubElements(subElements);

		//path = null;
		for (j = 0, d = subElements.size(); j < d && success == false; j++) {
			if (!subElements[j])
				continue;
			mc = subElements[j]->getComponent();
			if (!mc->isShowing())
				continue;
			cWidth = mc->getWidth();
			cHeight = mc->getHeight();
			p.x(screenX);
			p.y(screenY);
			// SwingUtilities.convertPointFromScreen(p, mc); TODO
			p = mc->getLocationOnComponent(p);
			/** Send the event to visible menu element if menu element currently in
			 *  the selected path or contains the event location
			 */
			if ((p.x() >= 0 && p.x() < cWidth && p.y() >= 0 && p.y() < cHeight))
			{
				int k;
				if (path.empty()) {
					path.resize(i + 2, IMenuElement::Ptr());
					for (k = 0; k <= i; k++)
						path[k] = tmp.at(k);
				}
				path[i + 1] = subElements[j];
				const MenuElements & currentSelection = selection; // = getSelectedPath();

				// Enter/exit detection -- needs tuning...
				if (currentSelection[currentSelection.size() - 1]
						!= path[i + 1] && (currentSelection.size() < 2
						|| currentSelection[currentSelection.size() - 2]
								!= path[i + 1]))
				{

					AComponentPtr oldMC = currentSelection[currentSelection.size()
							- 1]->getComponent();

					MouseEvent exitEvent = MouseEvent(oldMC, p,
							ev.getButtons(), MouseEvent::MOUSE_EXITED);
					currentSelection[currentSelection.size() - 1]->processMouseEvent(
							exitEvent, path, *this);

					MouseEvent enterEvent = MouseEvent(mc, p,
							ev.getButtons(), MouseEvent::MOUSE_ENTERED);
					subElements[j]->processMouseEvent(enterEvent, path, *this);
				}
				MouseEvent mouseEvent = MouseEvent(mc,  p,
						ev.getButtons(), ev.getType());
				subElements[j]->processMouseEvent(mouseEvent, path, *this);
				success = true;
			}
		}
	}
}
}}} // namespace(s)
