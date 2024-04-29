#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "card.h"


// Pretty print is given for you

void pretty_print(POKEMON_T* pokemon, FILE* fp_ascii); // prints the pokemon data in a nice format. Uses ascii.bin (ascii art) if it is provided and not null.

//Instantiating abilities and pokemons as global variables
// You will need to implement these two functions:

ABILITY_T* parse_ability(char * buf); // Parses a line from abilities.csv and returns an ABILITY_T*. Parsing means to analyze a string and break it down into its components.
// Example: "1,Stench,The stench may cause the target to flinch.," -> ABILITY_T* with name = "Stench" and desc = "The stench may cause the target to flinch.
POKEMON_T* parse_pokemon(char * buf, ABILITY_T** abilities, size_t total_abilities); // Parses a line from pokemon.csv returns a POKEMON_T*
// Example: "1,Bulbasaur,Grass,Poison,Overgrow,Chlorophyll,1,49,49,65,65,45,1,0.7,0.7" -> POKEMON_T* with id = 1, name = "Bulbasaur", type = GRASS, subtype = POISON, ability_one = ABILITY_T* with name = "Overgrow", hp = 49, attack = 49, defense = 65, special_attack = 65, special_defense = 45, speed = 45, base_experience = 1, weight = 0.7, height = 0.7

// Function to convert type string to Type enum

Type get_type(const char* typeStr) {
    if (strcmp(typeStr, "Normal") == 0) return NORMAL;
    if (strcmp(typeStr, "Fire") == 0) return FIRE;
    if (strcmp(typeStr, "Water") == 0) return WATER;
    if (strcmp(typeStr, "Electric") == 0) return ELECTRIC;
    if (strcmp(typeStr, "Grass") == 0) return GRASS;
    if (strcmp(typeStr, "Ice") == 0) return ICE;
    if (strcmp(typeStr, "Fighting") == 0) return FIGHTING;
    if (strcmp(typeStr, "Poison") == 0) return POISON;
    if (strcmp(typeStr, "Ground") == 0) return GROUND;
    if (strcmp(typeStr, "Flying") == 0) return FLYING;
    if (strcmp(typeStr, "Psychic") == 0) return PSYCHIC;
    if (strcmp(typeStr, "Bug") == 0) return BUG;
    if (strcmp(typeStr, "Rock") == 0) return ROCK;
    if (strcmp(typeStr, "Ghost") == 0) return GHOST;
    if (strcmp(typeStr, "Dragon") == 0) return DRAGON;
    if (strcmp(typeStr, "Dark") == 0) return DARK;
    if (strcmp(typeStr, "Steel") == 0) return STEEL;
    if (strcmp(typeStr, "Fairy") == 0) return FAIRY;
    return NONE; // Default case if no match is found
}

// Making these global will make things easier

ABILITY_T** Abilities; // this is an array of pointers to ABILITY_T named abilities (you will need to malloc this)
size_t total_abilities; // this is the total number of abilities in the array above
POKEMON_T** Pokedex; // this is an array of pointers to POKEMON_T which happens to be pokemon.csv
size_t total_pokemons; // this is the total number of pokemons in the array above


// This is used when sorting the abilities by name

int sort_abilities(const void *a, const void *b) {
    ABILITY_T* ability_a = *((ABILITY_T**)a); // casting 'a' from const void* to ABILITY_T*
	ABILITY_T *ability_b = *((ABILITY_T**)b); // casting 'b' from const void* to ABILITY_T*
    return strcmp(ability_a->name, ability_b->name);
}


// This is used when sorting the pokemon by name

int sort_pokemon(const void *a, const void *b) {
    POKEMON_T* pokemon_a = *(POKEMON_T**)a; // casting 'a' from const void* to POKEMON_T*
    POKEMON_T *pokemon_b = *(POKEMON_T**)b; // casting 'b' from const void* to POKEMON_T*
	return strcmp(pokemon_a->name, pokemon_b->name);
}


// this is used when comparing the string of the ability in the
// pokemon buf in parse_pokemon to the abilities array entries

int search_comp(const void *a, const void *b) {
    char* ability_name = (char*)a;
    ABILITY_T* ability = *(ABILITY_T**)b;
    return strcmp(ability_name, ability->name);

}


int main(int argc, char *argv[]) {

    // the 0 or 1 signifies if you will draw the ascii art or not
    
    if (argc != 3) {
        printf("usage: ./parser pokemon_csv (0 || 1)\n");
        return -1;
    }
    if(*argv[2] != '0' && *argv[2] != '1') {
        printf("usage: ./parser pokemon_csv (0 || 1)\n");
        return -2;
    }

    // Allocating memory for the array of pointers to POKEMON_T and ABILITY_T
    
    Pokedex = malloc(sizeof(POKEMON_T*)); // allocate memory for the array of pointers to POKEMON_T
    Abilities = malloc(sizeof(ABILITY_T*)); // allocate memory for the array of pointers to ABILITY_T

    // Initializing number of pokemons and abilities to 0
    
    size_t total_pokemons = 0;
    
    size_t total_abilities = 0;
    
    
    // Initializing the variables for the getline function
    
    char *line = NULL; // variable used for getline, acts as a buffer
    
    size_t len = 0; // variable used for getline, return length of characters read


    // TODO open abilities.csv, read it into ABILITY_T** array for later
    
    char *abilities_fd = "abilities.csv"; // open the file
   
    FILE *abilities_file = fopen(abilities_fd, "r"); // open the file
   
    getline(&line, &len, abilities_file); // read the first line to skip the header
   
    ssize_t get_line = 0; // variable to store the return value of getline
   
    while((get_line = getline(&line, &len, abilities_file)) != -1) {  // read the rest of the lines
        
        if (get_line >0){
            Abilities = realloc(Abilities, (total_abilities + 1) * sizeof(ABILITY_T)); // allocate memory for the array of pointers to ABILITY_T
        

            if (Abilities == NULL) {
                fprintf(stderr, "Error: Memory allocation failed\n");
                return -3;
            }

            Abilities[total_abilities] = parse_ability(line); // parse the line and store the ABILITY_T* in the array
        
            total_abilities++; // increment the number of abilities
        }
    }

    // TODO sort the ABILITY_T** array (???)
    
    qsort(Abilities, total_abilities, sizeof(ABILITY_T*), sort_abilities); // sort the ABILITY_T** array
   
    // TODO open argv[1], read it into POKEMON_T** array for later
    char *pokemon_fd = argv[1]; // open the file
    FILE *pokemon_file = fopen(pokemon_fd, "r"); // open the file
if (!pokemon_file) { // Check if file was successfully opened
    fprintf(stderr, "Could not open file %s\n", pokemon_fd);
    
    return -1; // Or appropriate error handling
}


    
    getline(&line, &len, pokemon_file); // read CSV header to skip it

    

    while((get_line = getline(&line, &len, pokemon_file)) != -1)
    {
        
        if (get_line>0){
        POKEMON_T* new_pokemon = NULL; // New POKEMON_T* pointer to a pokemon struct

        new_pokemon = parse_pokemon(line, Abilities, total_abilities); // parse the line and store the POKEMON_T* in the array
        
            if (new_pokemon != NULL) {
                Pokedex = realloc(Pokedex, (total_pokemons + 1) * sizeof(POKEMON_T*)); // allocate memory for the array of pointers to POKEMON_T
                
                if (Pokedex == NULL) {
                    fprintf(stderr, "Error: Memory allocation failed\n");
                    return -3;
                }
                
                Pokedex[total_pokemons] = new_pokemon; // store the POKEMON_T* in the array
                
                total_pokemons++; // increment the number of pokemons
            }
        line = NULL; // reset the line

        len = 0; // reset the length
        }
    }

    
    // TODO sort POKEMON_T** array
    qsort(Pokedex, total_pokemons, sizeof(POKEMON_T*), sort_pokemon);

    // TODO open ascii.bin
    FILE *ascii_file = NULL; // open the file

    if (*argv[2] == '1') {
    
        FILE *ascii_file = fopen("ascii.bin", "r"); // open the file
        // TODO output the pokemon data, free card immediately after it is printed
   
        for (int i = 0; i <= total_pokemons - 1; i++) {
            pretty_print(Pokedex[i], ascii_file);
            free(Pokedex[i]);
        }
        if (ascii_file == NULL) {
            fprintf(stderr, "Error: Could not open ascii.bin\n");
            return -4;
        }
        if (ascii_file != NULL) {
        fclose(ascii_file);
    }
    return 0;
        
    }
    else {
        for (int i = 0; i <= total_pokemons - 1; i++) {
            pretty_print(Pokedex[i], NULL);
            free(Pokedex[i]);
        }
    }
    // TODO cleanup the rest of resources used in program
    free(Pokedex);
    free(Abilities);
    free(line);
    fclose(abilities_file);
    fclose(pokemon_file);
    if(ascii_file != NULL) {
        fclose(ascii_file);
    }
}

// this function has to allocate an ABILITY_T* (an ability pointer) for the ability
// then call strsep to parse out the name and description
// from the buffer text, and return
ABILITY_T* parse_ability(char * buf) { // Parsing a line from abilities.csv and returning an ABILITY_T*. Parsing means to analyze a string and break it down into its components.
   
    ABILITY_T* ability = malloc(sizeof(ABILITY_T)); // allocate memory for the ABILITY_T. Allocate memory for Abilitiies
    char *name = NULL; // name is a pointer to the name of the ability

    char *desc = NULL; // desc is a pointer to the description of the ability

    name = strsep(&buf, ",\n\0"); // parse the name of the ability

    if (strstr(name,"-") != NULL) // if the name contains a hyphen
    {
        char *temp = malloc(strlen(name) + 1); // allocate memory for the temp variable

        char *sep = strsep(&name, "-"); // parse the first part of the name

        memset(sep, (sep[0] - 32), 1); // convert the first letter to uppercase

        strcpy(temp, sep); // copy the first part of the name to the temp variable

        while (name != NULL) // while the name is not null
        {
            strcat(temp," "); // concatenate a space to the temp variable
            
            sep = strsep(&name, "-"); // parse the next part of the name

            if (sep[0] >= 97 && sep[0] <= 122) // if the first letter is lowercase
            {
                memset(sep, (sep[0] - 32), 1); // convert the first letter to uppercase
            }

            strcat(temp, sep); // concatenate the next part of the name to the temp variable
        }

        ability->name = strdup(temp); // Duplicate the temp variable and store it in the name of the ability
    }

    else // if the name does not contain a hyphen
    {
        if (name[0] >= 97 && name[0] <= 122) // if the first letter is lowercase
        {
            memset(name, (name[0] - 32), 1); // convert the first letter to uppercase
        }

        ability->name = strdup(name); // Duplicate the name and store it in the name of the ability
    }

    desc = strsep(&buf, "\n\0"); // parse the description of the ability

    ssize_t len = strlen(desc); // store the length of the description

    desc[len - 1] = '\0'; // remove the newline character from the end of the description

    ability->desc = strdup(desc); // Duplicate the description and store it in the desc of the ability

    return ability; // return the ABILITY_T*
} 

// this function has a lot going on it it
// once you have the parsed to the id, if it is missing return null as 
// this means it is an empty line. if it is there, then you can continue.
// allocate a POKEMON_T*, and start filling out the data in it.
// for the abilities, once the name is read search for it in the abilities array.
// the weight and height fields have to be converted to the right unit
// of measurement, and finally into a proper float.
POKEMON_T* parse_pokemon(char * buf , ABILITY_T** abilities, size_t total_abilities) { // Parsing a line from pokemon.csv and returning a POKEMON_T*
    // Pokemon struct. A pokemon struct is a structure that contains information about a pokemon.
    // The information includes the pokemon's name,id,type,subtype,ablty_one,ablty_two,ablty_three,base_exp,weight,height,hp,attack,defense,spec_attack,spec_defense,speed,offset
    // Read the pokemon.csv file and parse the data into a POKEMON_T*.
    

    POKEMON_T* pokemon = malloc(sizeof(POKEMON_T)); // allocate memory for the POKEMON_T
    
    char *token; // token is a pointer to the first token found in the string
    // Parse and filling each field of the POKEMON_T
   
   // Name Parsing
    token = strsep(&buf, ",\n"); 
    
    
    if (token[0] >= 97 && token[0] <= 122) // if the first letter is lowercase 
    {
        memset(token, (token[0] - 32), 1); // convert the first letter to uppercase
    }

    pokemon->name = strdup(token);

    // ID Parsing

    token = strsep(&buf, ",\n"); // parse the id

    if (strcmp(token, "") == 0) {
       // fprintf(stderr, "Error: Missing ID\n");
        return NULL;
    }

    pokemon->id = atoi(token); // store the id (atoi converts a string to an integer)

    

    // Type & Subtype Parsing

    for (int i=0; i <= 1 ; i++)
    {
        token = strsep(&buf, ",\n"); // parse the type or subtype

        if (strcmp(token, "") == 0) {

            pokemon->subtype = NONE;
            
            break;
        }

        if (token[0] >= 97 && token[0] <= 122) // if the first letter is lowercase
        {
            memset(token, (token[0] - 32), 1); // convert the first letter to uppercase
        }

        if (i == 0)
        {
            pokemon->type = get_type(token); // store the type
        }
        if (i == 1)
        {
            pokemon->subtype = get_type(token); // store the subtype
        }  
    }
    

    // Ability Parsing

    pokemon->ability_one = NULL; // set the first ability to NULL
    pokemon->ability_two = NULL; // set the second ability to NULL
    pokemon->ability_three = NULL; // set the third ability to NULL

    for (int i = 0; i < 3; i++) { // loop through abilities

        ABILITY_T *search_ability = NULL; // set the search ability to NULL

        token = strsep(&buf,",\n");

        if (strcmp(token, "") == 0) { // if the token is empty
            
            if (i == 1)
            {
                strsep(&buf,",\n"); // skip the hidden ability
            }

            break; // break the loop
        }

        char *realtoken = strdup(token); // duplicate the token and store it in realtoken

        char *temp = malloc(strlen(realtoken) + 1); // allocate memory for the temp variable
   
        *temp = '\0'; // set the first character of temp to null

        char *sep = strsep(&realtoken, "-"); // parse the first part of the token

        while (sep != NULL) // while sep is not null
        {   
            if (*sep)
            {

            if (sep[0] >= 97 && sep[0] <= 122) // if the first letter is lowercase
            {
                memset(sep, (sep[0] - 32), 1); // convert the first letter to uppercase
            }

            if (*temp){
                strcat(temp, " "); // concatenate a space to the temp variable
            }

            strcat(temp, sep); // concatenate the next part of the token to the temp variable
            }
            sep = strsep(&realtoken, "-"); // parse the next part of the token
            
        }
    
        search_ability = *(ABILITY_T**)bsearch(temp, abilities, total_abilities, sizeof(ABILITY_T*), search_comp); // search for the ability in the abilities array
    
        free(temp); // free the temp variable

        free(realtoken); // free the realtoken variable

        if (i == 0)
        {
            pokemon->ability_one = search_ability; // store the first ability in the POKEMON_T

            pokemon->ability_two = NULL; // set the second ability to NULL

            pokemon->ability_three = NULL; // set the third ability to NULL
        }
        if (i == 1)
        {
            pokemon->ability_two = search_ability; // store the second ability in the POKEMON_T

        }

        if (i == 2)
        {
            pokemon->ability_three = search_ability; // store the third ability in the POKEMON_T

        }
    }
    // Base Experience Parsing

    token = strsep(&buf, ","); // parse the base experience
    if (token == NULL || *token == '\0') { // if the token is empty
        fprintf(stderr, "Error: Missing base experience\n");
        free (pokemon);
        return NULL;
    }

    pokemon->base_experience = atoi(token); // store the base experience (atoi converts a string to an integer)
    
    // Weight Parsing

    token = strsep(&buf, ",\n"); // parse the weight
    
    if (token == NULL) {
        fprintf(stderr, "Error: Missing weight\n");
        return NULL;
    }
    
    pokemon->weight = atof(token) / 10; // store the weight (atof converts a string to a float)
    
    if (!pokemon->weight) {
        fprintf(stderr, "Error: Invalid weight\n");
        return NULL;
    }

    // Height Parsing

    token = strsep(&buf, ","); // parse the height
    
    if (token == NULL) {
        fprintf(stderr, "Error: Missing height\n");
        return NULL;
    }

    pokemon->height = atof(token) / 10; // store the height (atof converts a string to a float)
    
    if (!pokemon->height) {
        fprintf(stderr, "Error: Invalid height\n");
        return NULL;
    }

    // HP Parsing

    token = strsep(&buf, ",\n"); // parse the hp

    if (token == NULL) {
        fprintf(stderr, "Error: Missing hp\n");
        return NULL;
    }

    pokemon->hp = atoi(token); // store the hp (atoi converts a string to an integer)
    
    if (!pokemon->hp) {
        fprintf(stderr, "Error: Invalid hp\n");
        return NULL;
    }
    
    // Attack Parsing
    
    token = strsep(&buf, ",\n"); // parse the attack
    
    if (token == NULL) {
        fprintf(stderr, "Error: Missing attack\n");
        return NULL;
    }

    pokemon->attack = atoi(token); // store the attack (atoi converts a string to an integer)
    
    if (!pokemon->attack) {
        fprintf(stderr, "Error: Invalid attack\n");
        return NULL;
    }

    // Defense Parsing

    token = strsep(&buf, ",\n"); // parse the defense
    
    if (token == NULL) {
        fprintf(stderr, "Error: Missing defense\n");
        return NULL;
    }

    pokemon->defense = atoi(token); // store the defense (atoi converts a string to an integer)
    if (!pokemon->defense) {
        fprintf(stderr, "Error: Invalid defense\n");
        return NULL;
    }

    // Special Attack Parsing

    token = strsep(&buf, ",\n"); // parse the special attack
    if (token == NULL) {
        fprintf(stderr, "Error: Missing special attack\n");
        return NULL;
    }

    pokemon->special_attack = atoi(token); // store the special attack (atoi converts a string to an integer)
    if (!pokemon->special_attack) {
        fprintf(stderr, "Error: Invalid special attack\n");
        return NULL;
    }

    // Special Defense Parsing
    
    token = strsep(&buf, ",\n"); // parse the special defense
    if (token == NULL) {
        fprintf(stderr, "Error: Missing special defense\n");
        return NULL;
    }

    pokemon->special_defense = atoi(token); // store the special defense (atoi converts a string to an integer)
    if (!pokemon->special_defense) {
        fprintf(stderr, "Error: Invalid special defense\n");
        return NULL;
    }

    // Speed Parsing

    token = strsep(&buf, ",\n"); // parse the speed
    if (token == NULL) {
        fprintf(stderr, "Error: Missing speed\n");
        return NULL;
    }

    pokemon->speed = atoi(token); // store the speed (atoi converts a string to an integer)
    if (!pokemon->speed) {
        fprintf(stderr, "Error: Invalid speed\n");
        return NULL;
    }

    // Offset Parsing

    token = strsep(&buf, ",\n"); // parse the offset
   
    if (token == NULL) {
        fprintf(stderr, "Error: Missing offset\n");
        return NULL;
    }
    
    pokemon->offset = atoi(token); // store the offset (atoi converts a string to an integer)
    
    if (!pokemon->offset) {
        fprintf(stderr, "Error: Invalid offset\n");
        return NULL;
    }

    return pokemon; // return the POKEMON_T*
}
void pretty_print(POKEMON_T* pokemon, FILE* fp_ascii) {
    // PICTURE ////////////
    // if fp_ascii is null, that means its not supposed to display the art
    if(fp_ascii) {
        size_t data_len = 0;
        fseek(fp_ascii, pokemon->offset, SEEK_SET);
        fread(&data_len, sizeof(data_len), 1, fp_ascii);
        char *data_buf = malloc(data_len + 1);
        fread(data_buf, 1, data_len, fp_ascii);
        data_buf[data_len] = 0;
        printf("%s\n", data_buf);
        free(data_buf);
    }

    // ID, NAME & TYPE ////////////
    // print name & pokedex #
    printf("#%.03d ", pokemon->id);
    printf("%s\n", pokemon->name);
    // print type (and subtype if applicable)
    printf("\033[1m%s\033[0m", type_str[pokemon->type]);
    if (pokemon->subtype != NONE) {
        printf(" / \033[1m%s\033[0m\n", type_str[pokemon->subtype]);
    } else {
        printf("\n");
    }

    // ABILITIES ////////////
    // print abilities in a nice list
    printf("\n\033[1mAbilities:\033[0m");
    printf("\n1. %s\n", pokemon->ability_one->name);
    printf("   %s\n", pokemon->ability_one->desc);

    if(pokemon->ability_two){
        // sometimes the second ability is hidden
        if(pokemon->ability_three == NULL) {
            printf("\033[3m");
        }
        printf("2. %s", pokemon->ability_two->name);
        if(pokemon->ability_three == NULL) {
            printf(" (hidden)");
        }
        printf("\n   %s", pokemon->ability_two->desc);

        printf("\033[0m\n");
    }
    // third ability if it exists is guaranteed to be hidden. make italic
    if(pokemon->ability_three) {
        printf("\033[3m");
        printf("3. %s (hidden)\n", pokemon->ability_three->name);
        printf("   %s", pokemon->ability_three->desc);
        printf("\033[0m\n");
    }

    // remaining stat block
    printf("\nHP: %d\t\t| %.2fkg\n", pokemon->hp, pokemon->weight);
    if (pokemon->attack > 99) {
        printf("Atk: %d\t| %.2fm\n", pokemon->attack, pokemon->height);
    } else {
        printf("Atk: %d\t\t| %.2fm\n", pokemon->attack, pokemon->height);
    }
    if (pokemon->defense > 99) {
        printf("Def: %d\t|\n", pokemon->defense);
    } else {
        printf("Def: %d\t\t|\n", pokemon->defense);
    }
    printf("Sp. Atk: %d\t|\n", pokemon->attack);
    printf("Sp. Def: %d\t|\n", pokemon->special_defense);
    printf("Speed: %d\t| XP: %d\n\n", pokemon->speed, pokemon->base_experience);
}