#include <random>
#include <iostream> 
#include <string> 

#define MINIMUM_COMMAND_LINE_ARGUMENTS 4
#define FULL_ARGUMENT_LENGTH 5

#define MIN_COMMAND_LOCATION 1
#define MAX_COMMAND_LOCATION 2
#define WORD_AMMOUNT_COMMAND_LOCATION 3
#define LENGTH_COMMAND_LOCATION 4

#define EXIT_SUCCESS 0

int main(int argc, char ** argv){
    //will throw error if a command lineargument isnt a number
    //but this is just for my own experiment so its fine
    //if min is set to zero by user it will break
    if(argc >= MINIMUM_COMMAND_LINE_ARGUMENTS){        
        
        
        int min = std::stoi(argv[MIN_COMMAND_LOCATION]);
        int max = std::stoi(argv[MAX_COMMAND_LOCATION]);
        int wordAmmount = std::stoi(argv[WORD_AMMOUNT_COMMAND_LOCATION]);
        int length = 0;
        bool randomLength = true;;
        if(min == 0 || max == 0){
            randomLength = false;            
            length = std::stoi(argv[LENGTH_COMMAND_LOCATION]);
        }

        std::random_device engine;

        std::uniform_int_distribution<int> uniform_dist_length(min, max);
        std::uniform_int_distribution<int> uniform_dist_letters(0, 9);

        
        for(int i = 0; i < wordAmmount; ++i){
            if(randomLength){
                length = uniform_dist_length(engine);
            }
            for(int j = 0; j < length; ++j){
                std::cout << uniform_dist_letters(engine);
            }
            std::cout << std::endl;
        }
    }
    return EXIT_SUCCESS;
}