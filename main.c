#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAX_LENGTH 20 // Maximum password length according to azure
#define LOWER 'l'
#define UPPER 'u'
#define DIGIT 'd'
#define SPECIAL 's'
#define maxargs 7


#include <sys/time.h>

int minlength;
int maxlength;
int minlower;
int maxlower;
int minupper;
int maxupper;
int mindigit;
int maxdigit;
int minspecial;
int maxspecial;

// Warning, here be gpt
// Function to calculate the number of combinations of 'k' items from 'n' items
long long combinations(int n, int k) {
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    
    long long numerator = 1;
    long long denominator = 1;
    
    for (int i = 0; i < k; i++) {
        numerator *= (n - i);
        denominator *= (i + 1);
    }
    
    return numerator / denominator;
}

// Function to calculate the predicted number of valid masks
unsigned long long calculate_predicted_masks(int minlength, int maxlength,
                                    int minlower, int maxlower,
                                    int minupper, int maxupper,
                                    int mindigit, int maxdigit,
                                    int minspecial, int maxspecial) {
    
    // Neat little shortcut for if all are equal, thanks wolfram alpha!
    if (minlower == minupper && minupper == mindigit && mindigit == minspecial) {
    // Variables minlower, minupper, mindigit, and minspecial are all equal
        if (maxlength == maxlower && maxlower == maxupper && maxupper == maxdigit && maxdigit == maxspecial) {
            // Variables maxlength, maxlower, maxupper, maxdigit, and maxspecial are all equal
            return ((4.0 / 3.0) * (-1 + pow(4, maxlength)));
        }
    }

    long long total_masks = 0;
    
    // Iterate over each possible length of the password
    for (int length = minlength; length <= maxlength; length++) {
        long long valid_masks_length = 0;
        
        // Calculate possible combinations of lowercase, uppercase, digits, and specials
        for (int lower_count = minlower; lower_count <= maxlower; lower_count++) {
            for (int upper_count = minupper; upper_count <= maxupper; upper_count++) {
                for (int digit_count = mindigit; digit_count <= maxdigit; digit_count++) {
                    for (int special_count = minspecial; special_count <= maxspecial; special_count++) {
                        // Check if current combination satisfies the length requirement
                        int total_count = lower_count + upper_count + digit_count + special_count;
                        if (total_count == length) {
                            // Calculate number of valid combinations for this specific count
                            long long valid_combinations = combinations(length, lower_count)
                                                         * combinations(length - lower_count, upper_count)
                                                         * combinations(length - lower_count - upper_count, digit_count)
                                                         * combinations(length - lower_count - upper_count - digit_count, special_count);
                            valid_masks_length += valid_combinations;
                        }
                    }
                }
            }
        }
        
        // Add valid masks for this length to total masks
        total_masks += valid_masks_length;
    }
    
    return total_masks;
}
// Here end gpt




// Function to generate password masks efficiently
// Function to generate password masks based on criteria
void generate_password_masks(int minlength, int maxlength, int minlower, int minupper, int mindigit, int minspecial) {
    char mask[MAX_LENGTH + 1];  // Buffer to hold password mask
    int length, i, j;

    // Iterate over all possible lengths of the password
    for (length = minlength; length <= maxlength; length++) {
        // Iterate over all combinations of characters for the current length
        for (i = 0; i < (1 << (2 * length)); i++) {
            int lowercount = 0, uppercount = 0, digitcount = 0, specialcount = 0;

            // Generate the mask for the current combination
            for (j = 0; j < length; j++) {
                int shift = 2 * j;
                // Determine character type based on bitmask
                switch ((i >> shift) & 3) {
                    case 0:
                        mask[j] = LOWER;
                        lowercount++;
                        break;
                    case 1:
                        mask[j] = UPPER;
                        uppercount++;
                        break;
                    case 2:
                        mask[j] = DIGIT;
                        digitcount++;
                        break;
                    case 3:
                        mask[j] = SPECIAL;
                        specialcount++;
                        break;
                }
            }

            // Check if the mask satisfies the conditions
            if (lowercount >= minlower && uppercount >= minupper &&
                digitcount >= mindigit && specialcount >= minspecial) {
                // Null-terminate the mask string
                mask[length] = '\0';
                // Print the valid password mask
                printf("%s\n", mask);
            }
        }
    }
}

char* convert_bytes(unsigned long long bytes) {
    static char result[50]; // Static buffer for the result string (adjust size as needed)
    const char* units[] = {"Bytes", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB"};
    double size = (double)bytes;
    int unitIndex = 0;

    while (size >= 1024 && unitIndex < 8) {
        size /= 1024;
        unitIndex++;
    }

    sprintf(result, "%.2f %s", size, units[unitIndex]);
    return result;
}

void print_configuration_info(int minlength, int maxlength,
                              int minlower, int maxlower,
                              int minupper, int maxupper,
                              int mindigit, int maxdigit,
                              int minspecial, int maxspecial) {
    unsigned long long number_of_masks = calculate_predicted_masks(minlength, maxlength, minlower, maxlower, minupper, maxupper, mindigit, maxdigit, minspecial, maxspecial);
    char* size_prediction = convert_bytes(number_of_masks * 2);
    printf(
        "               Configuration Info\n"
        "╔═══════════════════════════════════════════════╗\n"
        "║                       Minimum         Maximum ║\n"
        "║\033[38;5;207m Length                %-16d%-8d\033[1;0m║\n"
        "║\033[38;5;33m Lowercase             %-16d%-8d\033[1;0m║\n"
        "║ Uppercase             %-16d%-8d║\n"
        "║\033[38;5;33m Digits                %-16d%-8d\033[1;0m║\n"
        "║\033[38;5;207m Special Characters    %-16d%-8d\033[1;0m║\n"
        "╟───────────────────────────────────────────────╢\n"
        "║ Total masks       %-28lld║\n"
        "║ Maskfile size     %-28s║\n"
        "╚═══════════════════════════════════════════════╝\n"
        ,
        minlength, maxlength, minlower, maxlower, minupper,
        maxupper, mindigit, maxdigit, minspecial, maxspecial, number_of_masks, size_prediction
        );

    if (number_of_masks > 100000000){
        printf("\033[1;31mWARN\033[1;0m: Detected ridiculous number of masks to be generated, this could take days/weeks!\n");
    }else if (number_of_masks > 10000000)
    {
        printf("\033[1;31mWARN\033[1;0m: Detected significant number of masks to be generated, this could take a lot of time!\n");
    }
}

void print_help(){
    printf(
        "Passwd_gen by Lilly ***REMOVED***\n"
        "Licensed under the GNU AGPLv3\n"
        "\n"
        "Error: No options provided, showing help.\n"

        "Arg, Required, Example\n"
        "minlength | yes | 6\n"
        "maxlength | yes | 14\n"
        "minlower  | yes | 1\n"
        "minupper  | yes | 1\n"
        "mindigit  | yes | 1\n"
        "minspecial| yes | 1\n"
        "\n"
        );

}



int main(int argc, char **argv) {
    if (argc == 1){
        print_help();
        return 0;
    }

    if(argc > maxargs){
        printf("Error, too many options provided! You provided %i, expected %i\n", (argc - 1), (maxargs - 1));
        exit(1);
    }
    if(argc < maxargs){
        printf("Error, too few options provided! You provided %i, expected %i\n", (argc - 1), (maxargs - 1));
        exit(1);
    }


    minlength = atoi(argv[1]);
    maxlength = atoi(argv[2]);
    minlower = atoi(argv[3]);
    minupper = atoi(argv[4]);
    mindigit = atoi(argv[5]);
    minspecial = atoi(argv[6]);

    int min_stable = minlower + minupper + mindigit + minspecial;
    if(min_stable > maxlength){
        printf("Error, max length given will not generate any masks due to minimums provided being greater than it!\n");
        exit(1);
    }

    print_configuration_info(minlength, maxlength, minlower, maxlower, minupper, maxupper, mindigit, maxdigit, minspecial, maxspecial);


    generate_password_masks(minlength, maxlength, minlower , minupper , mindigit , minspecial);


    return 0;
}