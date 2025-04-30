#include <iostream>
#include "trie.h"
#include <string>
#include <memory>
#include <sstream>

#include "comments_handler.h"
#include "lexer.h"

int main()
{
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
    std::regex char_regex("[a-zA-Z0-9_]");
    std::regex separators_regex("[\r\n\t ]+");
    std::istringstream ss("ashdbb\tsdas\n\ndsad\nasds");
    auto lex = lexer(&ss, 1, &char_regex, &separators_regex);
    for (auto l : lex)
    {
        if (std::holds_alternative<std::string>(l))
        {
            std::cout << std::get<std::string>(l) <<" ";
        }
        else
        {
            switch (std::get<lexer::CONTROL_CHAR_TYPES>(l))
            {
                case lexer::CONTROL_CHAR_TYPES::DEBUG:
                    std::cout<<"|DEBUG|";
                default: ;
            }
        }
    }
    std::cout<<"\nEnd lexer\n";


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