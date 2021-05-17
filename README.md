# Tenuis library
Tenuis is a collection of experimental utilities for C++11.

## Current state
This was an experiment to learn more about C++11. This is now unmaintained and should not be used.


## tenuis::optional
```cpp
template<class T, class TError = tenuis::nullopt_t>
class optional;
```
This class template is similar to [std::optional](http://en.cppreference.com/w/cpp/utility/optional) but extends its idea of being either a **value** or **empty** to being either a **value** or an **error**. It does so by adding a template parameter TError which is the type of the **error** when no **value** is present.

The default parameter for TError is tenuis::nullopt_t which is the exact equivalent of [std::nullopt_t](http://en.cppreference.com/w/cpp/utility/optional/nullopt_t). This allows tenuis::optional<T> to be used in the same way std::optional<T> would be used at no additional cost (storage or performance).

This is similar is some ways to using [boost::variant](http://www.boost.org/doc/libs/1_61_0/doc/html/variant.html) with two template parameters, but the implementation is a lot simpler and more opiniated about its content (i.e. variant has no name for its contained types).

### Example usage
```cpp
enum class ErrorCode { InvalidHello, InvalidWorld };

using ExampleResult = tenuis::optional<std::string, ErrorCode>;

ExampleResult makeExample(const std::string& hello, const std::string& world) {
	if (hello != "Hello")      return ErrorCode::InvalidHello;
	else if (world != "World") return ErrorCode::InvalidWorld;
	else                       return hello + " " + world + "!";
}

void printExample(const std::string& hello, const std::string& world) {
	auto example = makeExample(hello, world);

	if (example) {
		std::cout << "Result: " << example.value() << std::endl;
	} else if (example.error() == ErrorCode::InvalidHello) {
		std::cout << "Error: Invalid hello!" << std::endl;
	} else if (example.error() == ErrorCode::InvalidWorld) {
		std::cout << "Error: Invalid world!" << std::endl;
	}
}

// in code
printExample("Hello", "World");
printExample("Bonjour", "World");
printExample("Hello", "Monde");

// outputs
// Result: Hello World!
// Error: Invalid hello!
// Error: Invalid world!

```

### Main usage differences with std::optional
 * **has_error()** is the equivalent of **!has_value()** and indicates that an error is contained.
 * **error()** returns a reference to the contained error.
 * **value()** does not throw and accessing it while has_value() is false is undefined.
 * A few features are missing, some which could be implemented in the future.

### TODO
 * It may be possible to make this constexpr-compatible. The implementation for std::optional could serve as an example for this.
 * Tests and improvements.
 
### Limitations
Allowing to use the same type for T and TError template parameters could make sense, but it is not currently supported. This feature would add complexity to the code because something like SFINAE would be required to prevent redefinition of constructors and operators. Furthermore, disabling error constructors would mean we need a new way to construct an error.
