#include <jendefs.h>

#ifdef __cplusplus
extern "C" {
#endif // C++

/**
 * @defgroup grp_XORSRND 乱数生成(XorShift法)
 *
 */

/** @ingroup grp_XORSRND
 * XorShift 乱数生成法の初期化
 * - 東北大河田研による (http://meme.biology.tohoku.ac.jp/klabo-wiki/index.php?%B7%D7%BB%BB%B5%A1%2FC%2B%2B#y919a7e1)
 * @param s
 */
void XORSRND_vInitSeed(uint32 s) ;

/** @ingroup grp_XORSRND
 * XorShift 乱数生成法
 * - 事前に XORSRND_vInitSeed() により種を初期化しておくこと
 * - Marsaglia (July 2003). “Xorshift RNGs”. Journal of Statistical Software Vol. 8 (Issue  14).
 * @return 32bit乱数。
 */
uint32 XORSRND_u32GetRand();

#ifdef __cplusplus
}
#endif // C++