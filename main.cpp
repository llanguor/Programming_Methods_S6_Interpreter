#include <iostream>
#include "trie.hpp"
#include <string>
#include <memory>
#include <sstream>

#include "comments_handler.hpp"
#include "lexer.hpp"
#include<filesystem>

#include "interpreter.hpp"
#include "builder.hpp"

int main()
{
    /*
    std::string const alphabet = "abcdefghijklmnopqrsty";
    auto t (std::make_shared<trie<int>>(alphabet));
    t->upsert("", 1);
    t->upsert("keybc", 6);
    t->upsert("kkea", 5);
    t->upsert("kkek", 4);
    t->upsert("kya", 2);
    t->upsert("kyy", 3);
    t->upsert("kyya", 3);
    t->upsert("kyyy", 3);
    t->dispose("kyya");

    for (auto const & inf: *t)
    {
        std::cout<<"Node '" << inf->key <<"' = "<< inf->value << std::endl;
    }
    std::cout<<"End infix\n";
*/

  //  std::istringstream ss("He#l\nlo,[# world!\nas]d. Iasjdnsaj#DEBUG\nndjsandsad");

    /*
    std::istringstream ss("ashdbb\nsdasdsad\nasds");


    auto commentsh = comments_handler(&ss, 1);
    for (auto ch: commentsh)
    {
        if (std::holds_alternative<int>(ch))
        {
            std::cout << static_cast<char>(std::get<int>(ch));
        }
        else
        {
            switch (std::get<comments_handler::CONTROL_CHAR_TYPES>(ch))
            {
                case comments_handler::CONTROL_CHAR_TYPES::DEBUG:
                    std::cout<<"|DEBUG|";
                default: ;
            }
        }

    }
    */


    //std::cout<<std::filesystem::current_path();
    //std::istringstream ss("ashdbb\nsdasdsad\nasds");
   // std::fstream ss("../meta/input/settings.txt");

  /*  if (!ss.is_open())
    {
        std::cout<<"Failed to open settings file"<<std::endl;
    }*/

    /*
    auto lex = lexer(&ss, 1, "[a-zA-Z0-9_]", "[\r\n\t ]+");
    for (auto l : lex)
    {
        if (std::holds_alternative<std::string>(l))
        {
            std::cout << std::get<std::string>(l) <<" ";
        }
        else
        {
            switch (std::get<lexer::control_char_types>(l))
            {
                case lexer::control_char_types::debug:
                    std::cout<<"|DEBUG|";
                default: ;
            }
        }
    }
    std::cout<<"\nEnd lexer\n";
*/


    auto interpreter = interpreter::builder(
        "../meta/input/settings.txt",
        1);

    return 0;
}

/*
    std::istringstream ss("ashdbb\nsdasdsad\nasds");
    auto a = comments_handler(&ss, 1);


   auto in = a.begin();
   while (in != a.end())
   {

   }

   for (auto ch: a)
   {
       if (std::holds_alternative<int>(ch))
       {
           std::cout << static_cast<char>(std::get<int>(ch));
       }
       else
       {
           switch (std::get<comments_handler::CONTROL_CHAR_TYPES>(ch))
           {
               case comments_handler::CONTROL_CHAR_TYPES::DEBUG:
                   std::cout<<"|DEBUG|";
               default: ;
           }
       }

   }
   */