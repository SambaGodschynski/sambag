#include <sambag/disco/components/SvgComponent.hpp>
#include <sambag/disco/components/ui/SvgKnobUI.hpp>
#include <sambag/disco/components/DefaultBoundedRangeModel.hpp>

namespace sambag { namespace disco { namespace components {
namespace svgExtensions {

void installSvgKnobExtension(SvgComponent::Dummy::Ptr dummy) {
    DefaultBoundedRangeModel::Ptr model =
        dummy->getModel<DefaultBoundedRangeModel>();
    if (!model) {
        // do not override an existing model
        model = DefaultBoundedRangeModel::Ptr(new DefaultBoundedRangeModel());
        dummy->setModel(model);
    }
    dummy->setUserUI(ui::SvgKnobUI::create());
}


}
}}} // namespace(s)