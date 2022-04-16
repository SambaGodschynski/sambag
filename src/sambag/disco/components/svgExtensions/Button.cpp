#include <sambag/disco/components/SvgComponent.hpp>
#include <sambag/disco/components/ui/SvgButtonUI.hpp>
#include <sambag/disco/components/DefaultButtonModell.hpp>

namespace sambag { namespace disco { namespace components {
namespace svgExtensions {

void installSvgButtonExtension(SvgComponent::Dummy::Ptr dummy) {
    DefaultButtonModell::Ptr model =
        dummy->getModel<DefaultButtonModell>();
    if (!model) {
        // do not override an existing model
        model = DefaultButtonModell::Ptr(new DefaultButtonModell());
        dummy->setModel(model);
    }
    dummy->setUserUI(ui::SvgButtonUI::create());
    dummy->setName("SvgComponent::Dummy(#disco-button)");
}


}
}}} // namespace(s)