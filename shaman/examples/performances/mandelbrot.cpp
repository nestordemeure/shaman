// The Computer Language Benchmarks Game
// http://benchmarksgame.alioth.debian.org/
//
// contributed by Elam Kolenovic
//
// Changes (2013-05-07)
//   - changed omp schedule for more even distribution of work
//   - changed loop variables to signed integer because msvc was complaining
//     when omp was enabled
//   - replaced std::copy and std::fill by one loop. slightly faster.
//   - swapped order of accuracy_benchmarks in for-i-loop. slightly faster.
//
// Changes (2013-04-19)
//   - using omp
//   - use buffer and fwrite at end instead of putchar
//   - pre-calculate cr0[]
//   - rename variables and use underscore before the index part of the name
//   - inverted bit accuracy_benchmarks, better performance under MSVC
//   - optional argument for file output, usefull in windows shell
//
// Changes (2013-04-07):
//   - removed unnecessary arrays, faster especially on 32 bits
//   - using putchar instead of iostreams, slightly faster
//   - using namespace std for readability
//   - replaced size_t with unsigned
//   - removed some includes
//
// Source : http://benchmarksgame.alioth.debian.org/u64q/program.php?test=mandelbrot&lang=gpp&id=9

#include <cstdio>
#include <cstdlib>
#include <vector>

#include <Shaman.h>

typedef unsigned char Byte;

using namespace std;

int main(int argc, char *argv[])
{
    const int N = max(0, (argc > 1) ? atoi(argv[1]) : 16000);
    const int width = N;
    const int height = N;
    const int max_x = (width + 7) / 8;
    const int max_iterations = 50;
    const Sdouble limit = 2.0;
    const Sdouble limit_sq = limit * limit;

    vector<Byte> buffer(height * max_x);

    vector<Sdouble> cr0(8 * max_x);
    #pragma omp parallel for
    for (int x = 0; x < max_x; ++x)
    {
        for (int k = 0; k < 8; ++k)
        {
            const int xk = 8 * x + k;
            cr0[xk] = (Sdouble(2.0) * xk) / width - Sdouble(1.5);
        }
    }

    #pragma omp parallel for schedule(guided)
    for (int y = 0; y < height; ++y)
    {
        Byte *line = &buffer[y * max_x];

        const Sdouble ci0 = Sdouble(2.0) * y / height - Sdouble(1.0);

        for (int x = 0; x < max_x; ++x)
        {
            const Sdouble *cr0_x = &cr0[8 * x];
            Sdouble cr[8];
            Sdouble ci[8];
            for (int k = 0; k < 8; ++k)
            {
                cr[k] = cr0_x[k];
                ci[k] = ci0;
            }

            Byte bits = 0xFF;
            for (int i = 0; bits && i < max_iterations; ++i)
            {
                Byte bit_k = 0x80;
                for (int k = 0; k < 8; ++k)
                {
                    if (bits & bit_k)
                    {
                        const Sdouble cr_k = cr[k];
                        const Sdouble ci_k = ci[k];
                        const Sdouble cr_k_sq = cr_k * cr_k;
                        const Sdouble ci_k_sq = ci_k * ci_k;

                        cr[k] = cr_k_sq - ci_k_sq + cr0_x[k];
                        ci[k] = Sdouble(2.0) * cr_k * ci_k + ci0;

                        if (cr_k_sq + ci_k_sq > limit_sq)
                        {
                            bits ^= bit_k;
                        }
                    }
                    bit_k >>= 1;
                }
            }
            line[x] = bits;
        }
    }

    return 0;
}
