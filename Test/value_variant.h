#ifndef TMW_CORE_TYPES_VALUE_VARIANT_H_
#define TMW_CORE_TYPES_VALUE_VARIANT_H_

#include <variant>
#include <string>

namespace tmw {

namespace types_internal {

template<typename T>
struct IsValueVariantSupported {
	static const bool value =
		std::is_same_v<T, int> ||
		std::is_same_v<T, unsigned int> ||
		std::is_same_v<T, long long> ||
		std::is_same_v<T, unsigned long long> ||
		std::is_same_v<T, float> ||
		std::is_same_v<T, double> ||
		std::is_same_v<T, std::string>;
};

template<typename T>
inline constexpr bool kIsValueVariantSupported = IsValueVariantSupported<T>::value;

enum class ValueVariantSupportedType {
	kInt,
	kUInt,
	kLongLong,
	kULongLong,
	kFloat,
	kDouble,
	kString
};

template<typename T, std::enable_if_t<kIsValueVariantSupported<T>, int> = 0>
T ExtractAs(const std::string& value) {
	if (value.empty()) return T{};

	try {
		if constexpr (std::is_same_v<T, int>)
			return std::stoi(value);
		else if constexpr (std::is_same_v<T, unsigned int>)
			return std::stoul(value);
		else if constexpr (std::is_same_v<T, long long>)
			return std::stoll(value);
		else if constexpr (std::is_same_v<T, unsigned long long>)
			return std::stoull(value);
		else if constexpr (std::is_same_v<T, float>)
			return std::stof(value);
		else if constexpr (std::is_same_v<T, double>)
			return std::stod(value);
		else if constexpr (std::is_same_v<T, std::string>)
			return value;
		else
			return T{};
	}
	catch (...) {
		return T{};
	}
}

} // namespace types_internal

class ValueVariant {
public:
	using Type = types_internal::ValueVariantSupportedType;

	ValueVariant() = default;

	template<typename T, std::enable_if_t<types_internal::kIsValueVariantSupported<T>, int> = 0>
	ValueVariant(T&& value) : value_(std::forward<T>(value)) {}

	ValueVariant(const char* value) : value_(std::string(value)) {}

	Type GetType() const { return static_cast<Type>(value_.index()); }

	int ToInt() const { return ExtractAs<int>(); }

	unsigned int ToUInt() const { return ExtractAs<unsigned int>(); }

	long long ToLongLong() const { return ExtractAs<long long>(); }

	unsigned long long ToULongLong() const { return ExtractAs<unsigned long long>(); }

	float ToFloat() const { return ExtractAs<float>(); }

	double ToDouble() const { return ExtractAs<double>(); }

	std::string ToString() const { return ExtractAs<std::string>(); }

	template<typename U, std::enable_if_t<types_internal::kIsValueVariantSupported<U>, int> = 0>
	U ExtractAs() const {
		auto convert = [](const auto& value) -> U {
			using T = std::decay_t<decltype(value)>;

			if constexpr (std::is_same_v<U, std::string>) {
				if constexpr (std::is_same_v<T, std::string>) {
					return value;
				}
				else {
					return std::to_string(value);
				}
			}
			else {
				if constexpr (std::is_same_v<T, std::string>) {
					return types_internal::ExtractAs<U>(value);
				}
				else {
					return static_cast<U>(value);
				}
			}
		};

		return std::visit(convert, value_);
	}

private:
	std::variant<int, unsigned int, long long, unsigned long long, float, double, std::string> value_;
};

} // namespace tmw

#endif // TMW_CORE_TYPES_VALUE_VARIANT_H_
