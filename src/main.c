
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_LINE 500 /* Characters */

/**
 * @brief Get line from stdin
 * @param[out] buf Pointer to line buffer
 * @param[in] buf_sz The max size of the buffer
 * @return Written bytes to the buffer
 */
int get_line(char* buf, size_t buf_sz) {
    size_t written = 0;
    int c          = 0;

    while (c != '\n') {
        c = getchar();

        if (c == EOF || written >= buf_sz) {
            *buf = '\0';
            return EOF; /* We are done */
        }

        *buf++ = c;
        written++;
    }

    *buf = '\0';

    return written;
}

/**
 * @brief Returns the normalized version of a character
 * @param[in] ch Character to be normalized
 * @return Normalized and lowercase character
 *
 * @todo Use getwchar() instead of getchar()
 * https://www.ibm.com/docs/en/i/7.1?topic=functions-getwchar-get-wide-character-from-stdin
 */
int normalized(int ch) {
    switch (ch) {
        case 225: /* 'á' */
        case 224: /* 'à' */
        case 228: /* 'ä' */
        case 226: /* 'â' */
        case 229: /* 'å' */
        case 227: /* 'ã' */
            ch = 'a';
            break;
        case 193: /* 'Á' */
        case 192: /* 'À' */
        case 196: /* 'Ä' */
        case 194: /* 'Â' */
        case 197: /* 'Å' */
        case 195: /* 'Ã' */
            ch = 'A';
            break;
        case 233: /* 'é' */
        case 232: /* 'è' */
        case 235: /* 'ë' */
        case 234: /* 'ê' */
            ch = 'e';
            break;
        case 201: /* 'É' */
        case 200: /* 'È' */
        case 203: /* 'Ë' */
        case 202: /* 'Ê' */
            ch = 'E';
            break;
        case 237: /* 'í' */
        case 236: /* 'ì' */
        case 239: /* 'ï' */
        case 238: /* 'î' */
            ch = 'i';
            break;
        case 205: /* 'Í' */
        case 204: /* 'Ì' */
        case 207: /* 'Ï' */
        case 206: /* 'Î' */
            ch = 'I';
            break;
        case 243: /* 'ó' */
        case 242: /* 'ò' */
        case 246: /* 'ö' */
        case 244: /* 'ô' */
        case 248: /* 'ø' */
            ch = 'o';
            break;
        case 211: /* 'Ó' */
        case 210: /* 'Ò' */
        case 214: /* 'Ö' */
        case 212: /* 'Ô' */
        case 216: /* 'Ø' */
            ch = 'O';
            break;
        case 250: /* 'ú' */
        case 249: /* 'ù' */
        case 252: /* 'ü' */
        case 251: /* 'û' */
            ch = 'u';
            break;
        case 218: /* 'Ú' */
        case 217: /* 'Ù' */
        case 220: /* 'Ü' */
        case 219: /* 'Û' */
            ch = 'U';
            break;
        /* ----------------------------------------------------------------
         */
        case 255: /* 'ÿ' */
            ch = 'y';
            break;
        case 199: /* 'Ç' */
            ch = 'C';
            break;
        case 231: /* 'ç' */
            ch = 'c';
            break;
        case 163: /* '£' */
            ch = 'L';
            break;
        case 159: /* 'ƒ' */
            ch = 'f';
            break;
        case 241: /* 'ñ' */
            ch = 'n';
            break;
        case 209: /* 'Ñ' */
            ch = 'N';
            break;
        case 208: /* 'Ð' */
            ch = 'D';
            break;
        default:
            break;
    }

    if (ch >= 'A' && ch <= 'Z')
        ch += 'a' - 'A';

    return ch;
}

/**
 * @brief Entry point of the program
 * @param argc Number of arguments
 * @param argv Vector of string arguments
 * @return Exit code
 */
int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr,
                "Error: No string provided.\n"
                "Usage:\n"
                "    %s \"str\" < file.txt\n",
                argv[0]);
        return 1;
    }

    size_t line_sz = MAX_LINE;
    char* line     = calloc(line_sz, sizeof(char));

    for (int cur_line = 1; get_line(line, line_sz) != EOF; cur_line++) {
        /* Starting position in the target line of the match and current
         * match position inside the arg string */
        int match_start = 0;
        int match_pos   = 0;

        for (size_t line_p = 0; line_p < line_sz && line[line_p] != '\0';
             line_p++) {
            /* If the current char in the arg string did not match, reset
             * the pos.
             *
             * We need this check here so we check again after resetting,
             * e.g.: "House" -> "HoHouse" The 3rd char of the arg string,
             * 'u', did not match, but we still need to check if the target
             * string starts at that postion.
             */
            if (normalized(line[line_p]) != normalized(argv[1][match_pos])) {
                match_start = 0;
                match_pos   = 0;
            }

            /* TODO */
            printf("%d:%c\n", line[line_p], line[line_p]);

            if (normalized(line[line_p]) == normalized(argv[1][match_pos])) {
                /* First matched character, store */
                if (match_pos == 0)
                    match_start = line_p;

                match_pos++;

                /* Last, check if the arg string is finished (after
                 * increasing match_pos) */
                if (argv[1][match_pos] == '\0') {
#ifdef DEBUG
                    printf("Match found at line %d, pos: %d-%ld\n", cur_line,
                           match_start, line_p);
#endif
                    printf("%d:%s", cur_line, line);
                }
            }
        }
    }

    free(line);
    return 0;
}
