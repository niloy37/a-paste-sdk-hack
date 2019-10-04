#pragma once
#pragma once

template<typename t>
class singleton {
public:
	template<typename... Args>
	static t* instance(Args&& ... args);
protected:
	singleton() = default;
	~singleton() = default;
};

template<typename t>
template<typename... Args>
t* singleton<t>::instance(Args&& ... args)
{
	static t instance(std::forward<Args>(args)...);
	return &instance;
}