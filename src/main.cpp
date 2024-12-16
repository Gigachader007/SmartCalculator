#include <iostream>

#include "CalcGraph/CalcGraph.hpp"

#include <cmath>


const float Pi = 3.14159265358f;

int main(){
    
    CalcGraph graph;
    graph.Register("cos",   [](const float t) -> const float {return std::cos(t);});
    graph.Register("sin",   [](const float t) -> const float {return std::sin(t);});
    graph.Register("tg",    [](const float t) -> const float {
        if(Pi*int((t - (Pi/2.f))/Pi) == t - (Pi/2.f)) throw std::runtime_error("тангенс не определён!");
        return std::tan(t);
    });
    graph.Register("ctg",   [](const float t) -> const float {
        if(Pi*int(t/Pi) == t) throw std::runtime_error("котангенс не определён!");
        return std::tan((Pi/2.f) - t);
    }); //ctg(x) = tg(pi/2 - x)
    graph.Register("ln",    [](const float t) -> const float {
        if(t <= 0.f) throw std::runtime_error("Логарифм не определён!");
        return std::log(t);
    });
    graph.Register("exp",   [](const float t) -> const float {return std::exp(t);});
    graph.Register("cosh",  [](const float t) -> const float {return std::cosh(t);});
    graph.Register("sinh",  [](const float t) -> const float {return std::sinh(t);});
    graph.Register("tanh",  [](const float t) -> const float {return std::tanh(t);});

    graph.Register("coth",  [](const float t) -> const float {
        if(t == 0.f) throw std::runtime_error("Гиперболический котангенс не определён!");
        return 1.f / std::tanh(t);
    });
    graph.Register("sqrt",  [](const float t) -> const float {
        if(t < 0.f) throw std::runtime_error("Корень для отрицательных чисел не определён!");
        return std::sqrt(t);
    });
    graph.Register("arcsin",[](const float t) -> const float {
        if(std::abs(t) > 1.f) throw std::runtime_error("Арксинус не определён для таких чисел!");
        return std::asin(t);
    });
    graph.Register("arccos",[](const float t) -> const float {
        if(std::abs(t) > 1.f) throw std::runtime_error("Арккосинус не определён для таких чисел!");
        return std::acos(t);
    });

    graph.Register("arctg", [](const float t) -> const float {return std::atan(t);});
    graph.Register("arcctg",[](const float t) -> const float {return 3.14159265358 / 2.0 - std::atan(t);});


    std::string function_line = "";
    std::cout << "Введите выражение: ";
    std::cin >> function_line;
    bool flag = false; //contains x
    for(auto i = 0; i < function_line.size(); ++i){
        if(function_line[i] == 'x'){
            auto c1 = function_line[std::min(std::max(i-1,0),(int)function_line.size()-1)];
            auto c2 = function_line[std::min(std::max(i+1,0),(int)function_line.size()-1)];
            for(auto j : {c1,c2}){
                if(j == '+' || j == '-' || j == ')' || j == '(' || j == '*' || j == '/' || j == '^'){
                    flag = true;
                    break;
                }
            }
            if(flag){
                break;
            }
        }
    }

    auto res_func = graph.Parse(function_line);

    if(!flag) std::cout << "Результат: " << res_func(0.f) << std::endl;
    else{
        std::cout << "Введите x=";
        float x;
        std::cin >> x;
        std::cout << "Результат: " << res_func(x) << std::endl;
    }
}
