/*
 * DiscoView.cpp
 *
 *  Created on: Mar 21, 2012
 *      Author: samba
 */
#include <sambag/disco/components/RootPane.hpp>
#include <sambag/disco/components/Button.hpp>
#include <sambag/disco/IDiscoFactory.hpp>
#include <sambag/disco/components/FramedWindow.hpp>
#include <sambag/disco/components/Panel.hpp>
#include <sambag/disco/components/Label.hpp>
#include <boost/assign/list_inserter.hpp>
#include <boost/foreach.hpp>
#include <map>
#include <float.h>

namespace sd = sambag::disco;
namespace sdc = sambag::disco::components;

void onNumericButton(void *src, const sdc::events::ActionEvent &ev);
void onOperatorButton(void *src, const sdc::events::ActionEvent &ev);
void onClearTxtField(void *src, const sdc::events::ActionEvent &ev);


sdc::FramedWindow::Ptr win;


static const std::string INPUT_LABEL = "inputLabel";
static const int INPUT_LABEL_SIZE = 40;

typedef float (*Operation)(float, float);
float sum = 0;
typedef std::map<std::string, Operation> OperatorMap;
OperatorMap opMap;
Operation currOp = NULL;
bool resetDigits = true;

namespace ops { // operations
	float add(float a, float b) {
		return a+b;
	}
	float sub(float a, float b) {
		return a-b;
	}
	float mul(float a, float b) {
		return a*b;
	}
	float div(float a, float b) {
		if (b==0.0) {
			return FLT_MAX;
		}
		return a/b;
	}
	float eq(float a, float b) {
		currOp = NULL;
		return a;
	}
	float ac(float a, float b) {
		return 0;
	}
}
void initOpMap() {
	using namespace boost::assign;
	insert(opMap)
	("+", &ops::add) ("-", &ops::sub)
	("*", &ops::mul) ("/", &ops::div)
	("=", &ops::eq) ("C", &ops::ac);
	//opMap.insert(std::make_pair("", &add));
}

void createCalcPane(sdc::WindowPtr win) {
	using namespace sambag::disco;
	using namespace sambag::disco::components;

	// numeric
	AContainer::Ptr con = Panel::create();
	con->setPreferredSize(Dimension(230,200));
	win->getRootPane()->add(con);
	const std::string nums[] = {"1", "2", "3", "4",
							  "5", "6", "7", "8", "9", "0", "."};
	const int NUM_NUMS = sizeof(nums) / sizeof(nums[0]);
	for (int i = 0; i < NUM_NUMS; ++i) {
		Button::Ptr btn = Button::create();
		btn->EventSender<sdc::events::ActionEvent>::
				addEventListener(&onNumericButton);
		btn->setText(nums[i]);
		btn->getFont().setFontFace("monospace");
		con->add(btn);
	}
	// operators
	initOpMap();
	BOOST_FOREACH(const OperatorMap::value_type &v, opMap) {
		Button::Ptr btn = Button::create();
		btn->EventSender<sdc::events::ActionEvent>::
				addEventListener(&onOperatorButton);
		btn->setText(v.first);
		btn->getFont().setFontFace("monospace");
		con->add(btn);
	}

	Label::Ptr label = Label::create();
	label->getFont().setFontFace("monospace").setSize(INPUT_LABEL_SIZE);
	label->setText("0");
	win->getRootPane()->add(label);
	win->getRootPane()->addTag(label, INPUT_LABEL);
}

void onClearTxtField(void *src, const sdc::events::ActionEvent &ev) {
}

std::string getInputText() {
	using namespace sambag::disco;
	using namespace sambag::disco::components;
	// get input label
	std::list<AComponentPtr> l;
	win->getRootPane()->getComponentsByTag(INPUT_LABEL, l);
	if (l.empty())
		return "";
	Label::Ptr label = boost::shared_dynamic_cast<Label>(l.back());
	if (!label)
		return "";
	return label->getText();
}
void setInputText(const std::string &txt, bool append = false) {
	using namespace sambag::disco;
	using namespace sambag::disco::components;
	// get input label
	std::list<AComponentPtr> l;
	win->getRootPane()->getComponentsByTag(INPUT_LABEL, l);
	if (l.empty())
		return;
	Label::Ptr label = boost::shared_dynamic_cast<Label>(l.back());
	if (!label)
		return;
	// set text
	std::string neuText;
	if (append)
		neuText = label->getText() + txt;
	else {
		neuText = txt;
	}
	label->setText(neuText);
	// resize if necessary
	const size_t MX_LENGTH = 5;
	if (neuText.length() > MX_LENGTH) {
		label->getFont().setSize( (MX_LENGTH / (float)neuText.length()) * INPUT_LABEL_SIZE );
	} else {
		label->getFont().setSize(INPUT_LABEL_SIZE);
	}
	win->getRootPane()->validate();
	win->getRootPane()->redraw();
}

void onNumericButton(void *src, const sdc::events::ActionEvent &ev) {
	using namespace sambag::disco::components;
	Button::Ptr b = boost::shared_dynamic_cast<Button>(ev.getSource());
	if (!b) {
		std::cout<<"!cast failed: "<<ev.getSource()->toString()<<std::endl;
		return;
	}
	setInputText(b->getText(), !resetDigits);
	if (resetDigits)
		resetDigits = false;
}

void onOperatorButton(void *src, const sdc::events::ActionEvent &ev) {
	using namespace sambag::disco::components;
	Button::Ptr b = boost::shared_dynamic_cast<Button>(ev.getSource());
	std::stringstream ss;
	float tmp;
	if (!b) {
		std::cout<<"!cast failed: "<<ev.getSource()->toString()<<std::endl;
		return;
	}
	Operation newOp = opMap[b->getText()];
	if (newOp == &ops::ac) {
		currOp = NULL;
		sum = 0;
		setInputText("0");
		resetDigits = true;
		return;
	}

	ss << getInputText();
	ss >> tmp;
	ss.str("");
	ss.clear();
	if (currOp) {
		sum = currOp(sum, tmp);
	} else {
		sum = tmp;
	}
	currOp = newOp;
	if (sum!=FLT_MAX) {
		ss << sum;
	} else {
		ss<<"NaN";
	}
	setInputText(ss.str());
	resetDigits = true;
}

int main() {
	using namespace sambag::disco;
	using namespace sambag::disco::components;
	win = sdc::FramedWindow::create();
	//win->setWindowBounds(Rectangle(100,100,230,200));
	win->setTitle("Calculator");
	createCalcPane(win);
	win->validate();
	win->pack();
	win->open();
	sdc::Window::startMainLoop();
	win.reset();
	std::cout<<"bye"<<std::endl;
}

