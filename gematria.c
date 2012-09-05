/* simple gematria calculator */

#include <stdio.h>
#include <stdbool.h>

/* expacts the string passed being UTF-8.
 * works natively on linux, but may require some work elsewhere...
 */

static int
gim2int(const unsigned char *word, bool final)
{
    int n = 0;

    while (*word != '\0')
    {
        switch (*word++)
        {
            case 0x90:
                n += 1;
                break;

            case 0x91:
                n += 2;
                break;

            case 0x92:
                n += 3;
                break;

            case 0x93:
                n += 4;
                break;

            case 0x94:
                n += 5;
                break;

            case 0x95:
                n += 6;
                break;

            case 0x96:
                n += 7;
                break;

            case 0x97:
                n += 8;
                break;

            case 0x98:
                n += 9;
                break;

            case 0x99:
                n += 10;
                break;

            case 0x9a:
                if (final)
                    n += 500;
                else

                    /* FALL */
                case 0x9b:
                n += 20;
                break;

            case 0x9c:
                n += 30;
                break;

            case 0x9d:
                if (final)
                    n += 600;
                else

                    /* FALL */
                case 0x9e:
                n += 40;
                break;

            case 0x9f:
                if (final)
                    n += 700;
                else

                    /* FALL */
                case 0xa0:
                n += 50;
                break;

            case 0xa1:
                n += 60;
                break;

            case 0xa2:
                n += 70;
                break;

            case 0xa3:
                if (final)
                    n += 800;
                else

                    /* FALL */
                case 0xa4:
                n += 80;
                break;

            case 0xa5:
                if (final)
                    n += 900;
                else

                    /* FALL */
                case 0xa6:
                n += 90;
                break;

            case 0xa7:
                n += 100;
                break;

            case 0xa8:
                n += 200;
                break;

            case 0xa9:
                n += 300;
                break;

            case 0xaa:
                n += 400;
                break;

            default:
                break; /* ignore */
        }
    }

    return n;
}

int
main(int argc, const char *argv[])
{
    bool final = false;

    for (int i = 1; i < argc; ++i)
    {
        if (*argv[i] == '-' && *++argv[i] == 'f')
        {
            final = true;
            continue;
        }

        printf("%d%s", gim2int((const unsigned char *)argv[i], final),
               (i < argc - 1) ? " " : "\n");
    }

    return 0;
}
