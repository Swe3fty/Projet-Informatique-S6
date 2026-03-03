#include <stdio.h>
#include <stdlib.h>
#include "regulation.h"
	 
float regulationTest(int regul,float consigne,float* tabT, int nT){
	float cmd = 100.0;
	float Kp = 1.1;
	float Ki = 0.2;
	float Kd = 0.15;
	if(regul == 1){
		if(tabT[nt-1] < consigne){
			cmd = 50.0;
		} else {
			cmd = 0.0;
		}
	}/* else {
		float P = Kp*(consigne - tabT[nT-1]);

	}*/


		
	return cmd;
}
    
  
