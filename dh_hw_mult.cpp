#include "systemc.h"
#include "digit.h"
#include "dh_hw_mult.h"

enum fsm_state{
	WAIT = 0,
	EXECUTE,
	OUTPUT,
	FINISH
};

fsm_state state = WAIT;

void dh_hw_mult::process_hw_mult()
{
	
  	NN_DIGIT a[2], b, c, t, u;
  	NN_HALF_DIGIT bHigh, bLow, cHigh, cLow;

  	for (;;) 
  	{

		switch(state)
		{
			case WAIT:
			// std::cout << "wait state (hw_mult)"  << std::endl;
			default:
				if (hw_mult_enable.read() == true) 
				{
					state = EXECUTE;
				}
				break;

			case EXECUTE:
				// std::cout << "execute state (hw_mult)"  << std::endl;
				// Read inputs	
				b = in_data_1.read();
				c = in_data_2.read();
				
				// Original code from NN_DigitMult()...		
				bHigh = (NN_HALF_DIGIT)HIGH_HALF (b);
				bLow = (NN_HALF_DIGIT)LOW_HALF (b);
				cHigh = (NN_HALF_DIGIT)HIGH_HALF (c);
				cLow = (NN_HALF_DIGIT)LOW_HALF (c);

				a[0] = (NN_DIGIT)bLow * (NN_DIGIT)cLow;
				t = (NN_DIGIT)bLow * (NN_DIGIT)cHigh;
				u = (NN_DIGIT)bHigh * (NN_DIGIT)cLow;
				a[1] = (NN_DIGIT)bHigh * (NN_DIGIT)cHigh;

				if ((t += u) < u) a[1] += TO_HIGH_HALF (1);
				u = TO_HIGH_HALF (t);

				if ((a[0] += u) < u) a[1]++;
				a[1] += HIGH_HALF (t);
				state = OUTPUT;
				wait();
				break;

			case OUTPUT:
			// std::cout << "output state (hw_mult)"  << std::endl;
				// Write outputs state
				out_data_low.write(a[0]);
				out_data_high.write(a[1]);
				hw_mult_done.write(true);
				state = FINISH;
				wait();

				break;

			case FINISH:
			// std::cout << "finish state (hw_mult)"  << std::endl;
				if (hw_mult_enable.read() == false) 
				{
					hw_mult_done.write(false);
					state = WAIT;
				}
				break;
					

		}


		wait();		// wait for a change of hw_mult_enable	

	}	

}
