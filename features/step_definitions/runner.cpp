#include <boost/test/unit_test.hpp>
#define EXPECT_EQ BOOST_CHECK_EQUAL

#include <cucumber-cpp/autodetect.hpp>

using cucumber::ScenarioScope;

#include <regex>
#include <memory>
#include <unordered_map>

struct tuple {
    double x, y, z, w;
};

struct test_proxy {
    virtual bool eval(std::string const s) = 0;
    virtual ~test_proxy() {};
};

template<typename T>
T parse_as(std::string const val_str);

template<>
double parse_as(std::string const val_str) {
    return std::stod(val_str);
}

template<typename T>
bool parse_compare(T&& t, std::string const op_str, std::string const val_str) {
    if (op_str == "=") {
        return t == parse_as<std::remove_reference_t<T>>(val_str);
    }
    return false;
}

struct tuple_proxy : public test_proxy {
    tuple t;

    tuple_proxy(std::string const s) {
        std::regex t4_re("(.+), (.+), (.+), (.+)");
        std::smatch m;
        std::regex_match(s, m, t4_re);

        t.x = std::stod(m[1].str());
        t.y = std::stod(m[2].str());
        t.z = std::stod(m[3].str());
        t.w = std::stod(m[4].str());
    }

    virtual bool eval(std::string const s) {
        static const std::regex comp_re("\\.(.+) (.+) (.+))");

        std::smatch m;
        if (std::regex_match(s, m, comp_re)) {
            double& field = t.x;
            if (m[1] == "y") {
                field = t.y;
            } else if (m[1] == "z") {
                field = t.z;
            } else if (m[1] == "w") {
                field = t.w;
            }
            return parse_compare(field, m[2], m[3]);
        }
        return true;
    }
};

struct proxy_factory {
    static std::unique_ptr<test_proxy> create(std::string const s) {
        static const std::regex tuple_re("tuple\\((.+)\\)");
        std::smatch m;
        if (std::regex_match(s, m, tuple_re)) {
            return std::make_unique<tuple_proxy>(m[1]);
        } else {
            return nullptr;
        }
    }
};

struct vars_ctx {
    std::unordered_map<std::string, std::unique_ptr<test_proxy>> vars;
};

// given assignment
GIVEN("^(.+) ← (.+)$") {
    ScenarioScope<vars_ctx> ctx;
    REGEX_PARAM(std::string, name);
    REGEX_PARAM(std::string, value);
    ctx->vars[name] = proxy_factory::create(value);
}

WHEN("test") {

}

// then member equals
THEN("^(.+)\\.(.+)$") {
    ScenarioScope<vars_ctx> ctx;
    REGEX_PARAM(std::string, name);
    REGEX_PARAM(std::string, expr);
    ctx->vars[name]->eval(expr);
}