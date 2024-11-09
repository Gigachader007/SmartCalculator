#pragma once

#include <string>
#include <functional>
#include <algorithm>
#include <map>

typedef std::function<const float(const float)> math_func_t;

class CalcGraph final {
    std::map<std::string,math_func_t> functions;
public:
    CalcGraph(){}
    ~CalcGraph(){}

    void Register(const std::string& name, const math_func_t& func){
        functions[name] = func;
    }

    std::function<const float(const float)> Parse(std::string line){
        line.erase(std::remove_if(line.begin(),line.end(),[](const unsigned char t){return std::isspace(t);}),line.end());
        if(line == ""){
            throw std::runtime_error("Передача пустой строки!");
        }
        //1. скобки
        //2. умножение/деление
        //3. сложение/вычитание
        {
            auto i = 0;
            auto counter = 0;
            for(auto sym : line){
                if((sym == '+' || sym == '-') && counter == 0){
                    auto sub_line1 = line.substr(0,i);
                    auto sub_line2 = line.substr(i+1,line.length());
                    if(sym == '+'){
                        return [=, self = this](const float t) -> const float {
                            return self->Parse(sub_line1)(t) + self->Parse(sub_line2)(t);
                        };
                    }
                    if(sym == '-'){
                        return [=, self = this](const float t) -> const float {
                            return self->Parse(sub_line1)(t) - self->Parse(sub_line2)(t);
                        };
                    }
                }
                if(counter < 0){
                    throw std::runtime_error("Очень много закрытых скобок!");
                }
                if(sym == '(') counter++;
                if(sym == ')') counter--;
                i++;
            }
            if(counter > 0){
                throw std::runtime_error("Очень много незакрытых скобок!");
            }
        }
        {
            auto i = 0;
            auto counter = 0;
            for(auto sym : line){
                if((sym == '*' || sym == '/') && counter == 0){
                    auto sub_line1 = line.substr(0,i);
                    auto sub_line2 = line.substr(i+1,line.length());
                    if(sym == '*'){
                        return [=, self = this](const float t) -> const float {
                            return self->Parse(sub_line1)(t) * self->Parse(sub_line2)(t);
                        };
                    }
                    if(sym == '/'){
                        return [=, self = this](const float t) -> const float {
                            return self->Parse(sub_line1)(t) / self->Parse(sub_line2)(t);
                        };
                    }
                }
                if(counter < 0){
                    throw std::runtime_error("Очень много закрытых скобок!");
                }
                if(sym == '(') counter++;
                if(sym == ')') counter--;
                i++;
            }
            if(counter > 0){
                throw std::runtime_error("Очень много незакрытых скобок!");
            }
        }

        bool flag = line.find_first_of('(') != std::string::npos && line.find_last_of(')') != std::string::npos;
        bool func_found = true;
        for(auto [name, func] : functions){
            if(flag){
                auto sss = line.substr(0,line.find_first_of('('));
                if(sss.empty()) break;
                func_found = false;
                if(sss == name){ //my function name and function name in std::map
                    func_found = true;
                    return [=, self = this](const float t) -> const float{
                        return func(self->Parse(line.substr(name.size(),line.size()-1))(t));
                    };
                }
            }
        }
        if(!func_found){
            throw std::runtime_error("Использована необъявленная функция!");
        }

        if(flag){
            return Parse(line.substr(1+line.find_first_of('('),line.find_last_of(')')-1));
        }
        
        if(line=="x"){
            return [](const float t){return t;};
        }
        return [line](const float){return std::stof(line);}; 
    }
};