#ifndef _SRC_UTILITIES_H
#define	_SRC_UTILITIES_H

#define  testbit(var, bit)     ((var) & (1 <<(bit)))
#define  setbit(var, bit)      ((var) |= (1 << (bit)))
#define  clearbit(var, bit)    ((var) &= ~(1 << (bit)))


#endif
