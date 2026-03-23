#ifndef CORE_H
#define CORE_H

#include "utils/vec2.h"
#include <memory>

template <typename T>
using Ref = std::shared_ptr<T>;

template <typename T, typename... Args>
auto make_Ref(Args&&... args) -> decltype(std::make_shared<T>(std::forward<Args>(args)...)) {
	return std::make_shared<T>(std::forward<Args>(args)...);
}



#endif
