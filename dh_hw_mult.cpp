#include "systemc.h"
#include "digit.h"
#include "dh_hw_mult.h"
#include "hardware.h"

/* New Hardware Implementation */

enum fsm_state{
	WAIT = 0,
	EXECUTE,
	UPDATE,
	OUTPUT,
	FINISH
};

fsm_state state = WAIT;

void dh_hw_mult::process_hw_mult()
{
  	for (;;) 
  	{

		switch(state)
		{
			case WAIT:
			// std::cout << "wait state (hw_mult)"  << std::endl;
			default:
				if (hw_mult_enable.read() == true) 
				{
					ldrb.write(true);
					ldrc.write(true);

					ldra0.write(false); 
					ldra1.write(false);

					state = EXECUTE;
				}
				break;

			case EXECUTE:
				// std::cout << "execute state (hw_mult)"  << std::endl;	
				
				ldra0.write(true); 
				ldra1.write(true);
				
				ldrb.write(false);
				ldrc.write(false);

				state = UPDATE;
				//wait();
				break;

			case UPDATE:
				ldra0.write(false); 
				ldra1.write(false);

				state = OUTPUT;
				break;

			case OUTPUT:
			// std::cout << "output state (hw_mult)"  << std::endl;
				// Write outputs state
				out_data_low.write(a0_sig.read());
				out_data_high.write(a1_sig.read());
				hw_mult_done.write(true);
				state = FINISH;
				//wait();

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