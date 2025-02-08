#include <memory>

template <template <typename...> class SubsystemType, typename... Listeners>
std::unique_ptr<SubsystemType<Listeners...>> factory_create_subsystem(Listeners*... listeners) {
    return std::make_unique<SubsystemType<Listeners...>>(listeners...);
}

// fixup for Cyclic Dependencies
template <typename SubsystemType, typename... NewListeners>
void factory_fixup_subsystem(SubsystemType& subsystem, NewListeners*... new_listeners) {
    subsystem.fixup(new_listeners...);
}