#ifndef BIT_MATH_H_
#define BIT_MATH_H_

#define SET_BIT(reg,bit) 		( reg |= (1 << bit) 	) 
#define CLR_BIT(reg,bit) 		( reg &= ~(1 << bit) 	)
#define TOG_BIT(reg,bit) 		( reg ^= (1 << bit) 	)
#define GET_BIT(reg,bit) 		((reg>>bit) & 1 		)
#define INS_BIT(reg,bit,value)		( reg = ( reg & ~(1 << bit) ) | (value << bit) )

#define SET_BYTE(reg, value) 	( reg = value			)

#define INS_FIELD(reg,mask,start,value) (reg = (  ( reg & ~(mask << start)) + (value << start) ) )

#define CONC_BIT(b7,b6,b5,b4,b3,b2,b1,b0) CONC_HELP(b7,b6,b5,b4,b3,b2,b1,b0)
#define CONC_HELP(b7,b6,b5,b4,b3,b2,b1,b0) 0b##b7##b6##b5##b4##b3##b2##b1##b0

#endif // BIT_MATH_H_
