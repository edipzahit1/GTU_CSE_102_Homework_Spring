#include <stdio.h>

int main(){
 
   int choice;
   int terminate = 0;
   int error = 0;
   int minError = 999;
   int R1, R2, R3, R4;
   int separator_x, separator_y;
   int regionclass;
   int best_R1, best_R2, best_R3, best_R4;
   int final_separator_x, final_separator_y;
   int min_sep_x, max_sep_x, min_sep_y, max_sep_y;
   
   while ( !terminate ) {
   
   printf ("Please enter your choice:\n");
   printf ("1- Explore Data:\n");
   printf ("2- Train Model:\n");
   printf ("3- Test Model:\n");
   printf ("4- Exit:\n");
  
   scanf ("%d", &choice);
   
   switch (choice) {
   
   case 1:
    
      FILE *dosya= fopen("data.txt", "r");
    
      if (dosya == NULL) {
        printf("Dosya acilamadi.\n");
        return 1;
        }
        
    int x, y, class;
    int max_x_0 = 0, max_y_0 = 0, min_x_0 = 999, min_y_0 = 999;
    int max_x_1 = 0, max_y_1 = 0, min_x_1 = 999, min_y_1 = 999;
        
        
   while( fscanf(dosya, "%d %d %d", &x, &y, &class) != EOF) {
   
     if (class == 0) {
   
       	 if ( x > max_x_0 ) { max_x_0 = x; }
  	   	 if ( y > max_y_0 ) { max_y_0 = y; }
 	   	 if ( x < min_x_0 ) { min_x_0 = x; }
  	   	 if ( y < min_y_0 ) { min_y_0 = y; }
   		
   }
   
    else if ( class == 1) {
         if ( x > max_x_1 ) { max_x_1 = x; }
		 if ( y > max_y_1 ) { max_y_1 = y; }
 		 if ( x < min_x_1 ) { min_x_1 = x; }
  		 if ( y < min_y_1 ) { min_y_1 = y; }
   
   
    }
    
   }
         
  fclose(dosya);
  
    printf ("Minimum and Maximum X and Y values:\n");
    printf ("Class 0: X in [%d %d], Y in [%d %d]\n", min_x_0, max_x_0, min_y_0, max_y_0);
    printf ("Class 1: X in [%d %d], Y in [%d %d]\n", min_x_1, max_x_1, min_y_1, max_y_1);

           break;
   case 2:
   
      FILE *ptr = fopen("data.txt", "r");
    
      if (ptr == NULL) {
        printf("Dosya acilamadi.\n");
        return 1;
        }
              
   while( fscanf(ptr, "%d %d %d", &x, &y, &class) != EOF) {
   
      if (class == 0) {
   
       	 if ( x > max_x_0 ) { max_x_0 = x; }
  	   	 if ( y > max_y_0 ) { max_y_0 = y; }
 	   	 if ( x < min_x_0 ) { min_x_0 = x; }
  	   	 if ( y < min_y_0 ) { min_y_0 = y; }
   		
      }
   
     else if ( class == 1) {
         if ( x > max_x_1 ) { max_x_1 = x; }
		 if ( y > max_y_1 ) { max_y_1 = y; }
 		 if ( x < min_x_1 ) { min_x_1 = x; }
  		 if ( y < min_y_1 ) { min_y_1 = y; }
   
     }
    
   }
   
   if (min_x_0 < min_x_1){
      min_sep_x = min_x_0;
      } else {
      min_sep_x = min_x_1;
      }
   if(max_x_0 > max_x_1){
      max_sep_x = max_x_0;
      } else {
      max_sep_x = max_x_1;
      }
   if (min_y_0 < min_y_1){
      min_sep_y = min_y_0;
      } else {
      min_sep_y = min_y_1;
      }
   if(max_y_0 > max_y_1){
      max_sep_y = max_y_0;
      } else {
      max_sep_y = max_y_1;
      }  
         
  fclose(ptr);  
  
   for ( separator_x = min_sep_x ; separator_x < max_sep_x; separator_x++){
     for( separator_y = min_sep_y ; separator_y < max_sep_y; separator_y++) {
        for (regionclass = 1; regionclass <= 16; regionclass++) {   
           
    	      switch (regionclass) {
       
			   case 1: R1 = 0, R2 = 0, R3 = 0, R4 = 1;
				      break;
			   case 2: R1 = 0, R2 = 0, R3 = 1, R4 = 0;
				      break;   
			   case 3: R1 = 0, R2 = 1, R3 = 0, R4 = 0;
				      break;    
			   case 4: R1 = 1, R2 = 0, R3 = 0, R4 = 0;
				      break;
			   case 5: R1 = 0, R2 = 0, R3 = 0, R4 = 0;
				      break;
			   case 6: R1 = 1, R2 = 1, R3 = 0, R4 = 0;
				      break;
			   case 7: R1 = 1, R2 = 0, R3 = 1, R4 = 0;
				      break;
			   case 8: R1 = 1, R2 = 0, R3 = 0, R4 = 1;
				      break;
			   case 9: R1 = 0, R2 = 0, R3 = 1, R4 = 1;
				      break;
			   case 10: R1 = 1, R2 = 1, R3 = 1, R4 = 0;
				      break;
			   case 11: R1 = 1, R2 = 1, R3 = 0, R4 = 1;
				      break;
			   case 12: R1 = 1, R2 = 0, R3 = 1, R4 = 1;
				      break;
			   case 13: R1 = 0, R2 = 1, R3 = 1, R4 = 1;
				      break;
			   case 14: R1 = 0, R2 = 1, R3 = 0, R4 = 1;
				      break;
			   case 15: R1 = 0, R2 = 1, R3 = 1, R4 = 0;
				      break;
			   case 16: R1 = 1, R2 = 1, R3 = 1, R4 = 1;
	       }
				
		     FILE *file;
		     file = fopen("data.txt", "r");
		             
			 if (file == NULL) {
				printf("Dosya acilamadi.\n");
				return 1;
			 }
    
             while( fscanf(dosya, "%d %d %d", &x, &y, &class) != EOF) {			   
					if( x <= separator_x && y > separator_y){						            
						        if ( class == R1 ) {
						        } else {
						        error++;
						        }
					}
					
					else  if( x > separator_x && y > separator_y){						           
						       if ( class == R2 ) {
						        } else {
						        error++;
						        }
					}
					
					
					else if( x < separator_x && y <= separator_y){
						        if ( class == R3 ) {
						        } else {
						        error++;
						        }
					}
					
					else if( x > separator_x && y <= separator_y){						           
						       if ( class == R4 ) {
						        } else {
						        error++;
						        }
				    }
					 					 
		     } //while
				   
				   if (error < minError) {
				   minError = error;
				   
				   final_separator_x = separator_x;
				   final_separator_y = separator_y;
				   best_R1 = R1;
				   best_R2 = R2;
				   best_R3 = R3;
				   best_R4 = R4;
				   
				   }
				   
				   fclose(file);
				   error = 0;
				 
	     } //region
	
	   } //sepy
	                          
	} //sepx
	                        
        printf("Separator Coordinates:%d %d", final_separator_x, final_separator_y);
        printf("Training error of the model: %d", minError);  
        printf("Labels of the regions:%d %d %d %d", best_R1, best_R2, best_R3, best_R4); 
        return 0; 
         
    
           break;
   case 3:
   
   
           break;    
   case 4:
           terminate = 1;
           printf ("exiting..");
           break;
                 
   default: printf ("please enter a valid choice!");
            break;
          
      
    
      }
      fflush(stdin);
   }
    return 0;
}
