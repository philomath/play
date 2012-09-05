/* output precentages of file types under the specified dir, pretty primitive */

#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ftw.h>

#if __GNUC__
# define UNUSED __attribute__((unused))
#else
# define UNUSED
#endif

#define USAGE "ftypes [option] [dirs]\nRead the source, dude!"

static int
calc_info(const char *fpath, const struct stat *sb, int type, struct FTW *ftwbuf UNUSED);

static int nreg, nblk, nchr, nfifo, nslink, nsock, ndir;

int
main(int argc, char *argv[])
{
    int flags, opt, nopend;
    flags = FTW_PHYS;  // don't follow symbolic links by default
    nopend = 20;       // number of dirs nftw should hold open simultaneously

    while ((opt = getopt(argc, argv, "dlmn:")) != -1)
    {
        switch (opt)
        {
            case 'd':
                flags |= FTW_DEPTH; // do a post-order traversal
                break;
            case 'l':
                flags &= ~FTW_PHYS; 
                break;
            case 'm':
                flags |= FTW_MOUNT;  // don't cross mount-points
                break;
            case 'n':
                nopend = atoi(optarg);
                break;
            default:
                puts(USAGE);
                return 1;
        }
    }
    
    do
    {
        if (nftw((optind < argc) ? argv[optind++] : ".", calc_info, 
            (nopend > 0) ? nopend : 20, flags) == -1)
        {
            perror("nftw");
            return 1;
        }
    }
    while (optind < argc);

    int ntot = nreg + ndir + nblk + nchr + nfifo + nslink + nsock;

    printf("total files    = %7d\n", ntot);
    ntot = (ntot) ? ntot : 1;       /* avoid divide by 0; print 0 for all counts */
    printf("regular files  = %7d, %g %%\n", nreg, nreg * 100.0 / ntot);
    printf("directories    = %7d, %g %%\n", ndir, ndir * 100.0 / ntot);
    printf("block special  = %7d, %g %%\n", nblk, nblk * 100.0 / ntot);
    printf("char special   = %7d, %g %%\n", nchr, nchr * 100.0 / ntot);
    printf("FIFOs          = %7d, %g %%\n", nfifo, nfifo * 100.0 / ntot);
    printf("symbolic links = %7d, %g %%\n", nslink, nslink * 100.0 / ntot);
    printf("sockets        = %7d, %g %%\n", nsock, nsock * 100.0 / ntot);
    return 0;
}

static int
calc_info(const char *fpath, const struct stat *sb, int type, struct FTW *ftwbuf UNUSED)
{
    switch (type)
    {
        case FTW_F:  /* it's a file. which one? */
            switch (sb->st_mode & S_IFMT)
            {
                case S_IFREG:
                    ++nreg; break;

                case S_IFBLK:
                    ++nblk; break;

                case S_IFCHR:
                    ++nchr; break;

                case S_IFIFO:
                    ++nfifo; break;

                /*case S_IFLNK:      // XXX not really returned as FTW_F in nftw
                    ++nslink; break; */

                case S_IFSOCK:
                    ++nsock; break;

                /*case S_IFDIR:
                    fprintf(stderr, "S_IFDIR for %s\n", fpath);
                    // directories should have type = FTW_D
                    break; */

                default:
                    fprintf(stderr, "no idea what %s is supposed to be?" 
                    "(st_mode & S_IFMT is %d)\n", fpath, sb->st_mode & S_IFMT);
            }

            break;

        case FTW_D: case FTW_DP:
            ++ndir; break;

        case FTW_DNR:
            fprintf(stderr, "can't read directory %s\n", fpath);
            ++ndir; break;

        case FTW_NS:
            fprintf(stderr, "stat error for %s\n", fpath);
            break;
            
        case FTW_SL:
            ++nslink; break;
            
        case FTW_SLN:
            fprintf(stderr, "%s is a dangling link\n", fpath);
            ++nslink; break;

        default:
            fprintf(stderr, "unknown type %d for pathname %s\n", type, fpath);
    }

    return 0;           /* To tell nftw() to continue */
}
