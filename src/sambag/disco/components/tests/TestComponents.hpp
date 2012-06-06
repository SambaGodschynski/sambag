/*
 * TestComponents.hpp TODO: rename to ComponentsForTest
 *
 *  Created on: May 22, 2012
 *      Author: samba
 */

#ifndef SAMBAG_TESTCOMPONENTS_HPP_
#define SAMBAG_TESTCOMPONENTS_HPP_

#include <sambag/disco/components/AContainer.hpp>
#include <sambag/disco/components/ALayoutManager.hpp>
#include <sambag/disco/components/Forward.hpp>
#include <sambag/com/events/PropertyChanged.hpp>
#include <sambag/disco/components/ui/AComponentUI.hpp>
#include <sambag/disco/components/ui/ALookAndFeel.hpp>
#include <string>

namespace tests {
	namespace sdc = sambag::disco::components;
	//=============================================================================
	template <typename T>
	class PropertyTestListener {
		PropertyTestListener() {}
	public:
		bool wasCalled;
		const std::string propertyName;
		T sollOld, sollNew, istOld, istNew;
		void propertyChanged(void *,
			const sambag::com::events::PropertyChanged &pc)
		{
			if (pc.getPropertyName() != propertyName)
				return;
			wasCalled = true;
			pc.getOldValue(istOld);
			pc.getNewValue(istNew);
		}
		PropertyTestListener(const std::string &propertyName, const T &sollOld,
				const T &sollNew) : wasCalled(false),
				propertyName(propertyName), sollOld(sollOld), sollNew(sollNew){}

		PropertyTestListener<T> createValidCopy()
		{
			PropertyTestListener<T> res = *this;
			res.wasCalled = true;
			res.istNew = sollNew;
			res.istOld = sollOld;
			return res;
		}
		bool operator==(const PropertyTestListener<T> &b) const {
			return wasCalled == b.wasCalled &&
					sollOld == b.sollOld &&
					istOld == b.istOld &&
					sollNew == b.sollNew &&
					istNew == b.istNew;
		}
		bool operator!=(const PropertyTestListener<T> &b) const {
			return !(*this == b);
		}
	};
	template <typename T>
	inline std::ostream & operator << (std::ostream &os, const PropertyTestListener<T> &pL) {
		os<<(pL.wasCalled ? "wasCalled" : "wasn't called");
		os<<" soll("<<pL.sollOld<<", "<<pL.sollNew<<"), ";
		os<<" ist("<<pL.istOld<<", "<<pL.istNew<<")";
		return os;
	}
	//=========================================================================
	class TestComponentUI : public sdc::ui::AComponentUI {
	public:
		typedef boost::shared_ptr<TestComponentUI> Ptr;
		static Ptr create() {
			return Ptr(new TestComponentUI());
		}
	};
	//=========================================================================
	class LookAndFeelForTest : public sdc::ui::ALookAndFeel {
	protected:
		LookAndFeelForTest();
	public:
		typedef boost::shared_ptr<LookAndFeelForTest> Ptr;
		static Ptr create() {
			return Ptr(new LookAndFeelForTest());
		}
	};
	//=============================================================================
	class TestComponent : public sdc::AComponent {
	protected:
		TestComponent() {}
		virtual void drawComponent(sambag::disco::IDrawContext::Ptr cn);
	public:
		sdc::ui::AComponentUIPtr getComponentUI() const { return ui; }
		sdc::ui::AComponentUIPtr getComponentUI(sdc::ui::ALookAndFeelPtr laf) const;
		static int numDrawCalled;
		typedef boost::shared_ptr<TestComponent> Ptr;
		static Ptr create() {
			using namespace sambag::disco;
			Ptr res(new TestComponent());
			res->self = res;
			return res;
		}
	};
	//=============================================================================
	class TestContainer : public sdc::AContainer {
	protected:
		TestContainer() {}
	public:
		sdc::ui::AComponentUIPtr getComponentUI(sdc::ui::ALookAndFeelPtr laf) const {
			return sdc::ui::AComponentUIPtr();
		}
		typedef boost::shared_ptr<TestContainer> Ptr;
		static Ptr create() {
			Ptr res(new TestContainer());
			res->self = res;
			return res;
		}
	};
	//=============================================================================
	class TestLayoutManager : public  sdc::ALayoutManager {
	public:
		typedef boost::shared_ptr<TestLayoutManager> Ptr;
	protected:
		TestLayoutManager() {}
	public:
		static Ptr create() {
			Ptr res(new TestLayoutManager());
			res->self = res;
			return res;
		}
		virtual void
		addLayoutComponent(sdc::AComponentPtr comp) {}
		virtual void
		layoutContainer(sdc::AContainerPtr parent) {}
		virtual sambag::disco::Dimension
		minimumLayoutSize(sdc::AContainerPtr parent) {
			return sambag::disco::Dimension();
		}
		virtual sambag::disco::Dimension
		maximumLayoutSize(sdc::AContainerPtr parent) {
			return sambag::disco::Dimension();
		}
		virtual sambag::disco::Dimension
		preferredLayoutSize(sdc::AContainerPtr parent) {
			return sambag::disco::Dimension();
		}
		virtual void
		removeLayoutComponent(sdc::AComponentPtr comp) {}
		void
		addLayoutComponentImpl(sdc::AComponentPtr comp, int c) {
		}
		virtual void invalidateLayout(sdc::AContainerPtr target) {}
		virtual sambag::disco::Coordinate
		getLayoutAlignmentY(sdc::AContainerPtr target) {
			return sambag::disco::Coordinate();
		}
		virtual sambag::disco::Coordinate
		getLayoutAlignmentX(sdc::AContainerPtr target) {
			return sambag::disco::Coordinate();
		}
		virtual std::string toString() const {
			return "TestLayoutManager";
		}
	}; // ALayoutManager
}

#endif /* TESTCOMPONENTS_HPP_ */
