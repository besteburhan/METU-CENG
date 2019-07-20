`timescale 1ns / 1ps
module FUNCROM (input [3:0] romAddress, output reg[4:0] romData);

/*Write your code here*/
  always@(*)
  begin
    if(romAddress==4'b0000)      romData = 5'b00000;
    else if(romAddress==4'b0001) romData = 5'b00010;
    else if(romAddress==4'b0010) romData = 5'b00100;
    else if(romAddress==4'b0011) romData = 5'b00111;
    else if(romAddress==4'b0100) romData = 5'b01010;
    else if(romAddress==4'b0101) romData = 5'b01011;
    else if(romAddress==4'b0110) romData = 5'b01101;
    else if(romAddress==4'b0111) romData = 5'b01110;
    else if(romAddress==4'b1000) romData = 5'b10001;
    else if(romAddress==4'b1001) romData = 5'b10010;
    else if(romAddress==4'b1010) romData = 5'b10100;
    else if(romAddress==4'b1011) romData = 5'b10111;
    else if(romAddress==4'b1100) romData = 5'b11001;
    else if(romAddress==4'b1101) romData = 5'b11010;
    else if(romAddress==4'b1110) romData = 5'b11110;
    else if(romAddress==4'b1111) romData = 5'b11111;


  end
endmodule
																						
module FUNCRAM (input mode,input [3:0] ramAddress, input [4:0] dataIn,input op, input [1:0] arg,  input CLK, output reg [8:0] dataOut);
/*Write your code here*/

 integer argDec;
 integer res;
 reg [8:0] dOut;
  reg [8:0] fRam [15:0];
 
 initial 
  begin
   argDec =0;
   res=0;
    fRam[15]=9'b000000000;
    fRam[14]=9'b000000000;
    fRam[12]=9'b000000000;
    fRam[11]=9'b000000000;
    fRam[10]=9'b000000000;
    fRam[9]=9'b000000000;
    fRam[8]=9'b000000000;
    fRam[7]=9'b000000000;
    fRam[6]=9'b000000000;
    fRam[5]=9'b000000000;
    fRam[4]=9'b000000000;
    fRam[3]=9'b000000000;
    fRam[2]=9'b000000000;
    fRam[1]=9'b000000000;
    fRam[0]=9'b000000000;
       
	
  end
  
  always@(*)
    begin
      if(mode==0)
       
        dataOut= fRam[ramAddress];
    end
  
  always@(posedge CLK )
   begin
	res=0;
     if(arg == 2'b00) argDec=2;
     else if(arg == 2'b01) argDec=1;
     else if(arg == 2'b10) argDec = -1;
     else if(arg == 2'b11) argDec = -2;
    if(mode==1)//write mode
	  begin
	   if(op == 0) //modulo
	    begin
	 	 if(dataIn[4]==0)
	 	   res = res + argDec*argDec*argDec*argDec;
	 	 else 
		   res = res -argDec*argDec*argDec*argDec;
	 	 if(dataIn[3]==0)
		   res = res + argDec*argDec*argDec;
		 else 
		   res = res -argDec*argDec*argDec;
		 if(dataIn[2]==0)
		   res = res + argDec*argDec;
		 else 
		   res = res -argDec*argDec;
		 if(dataIn[1]==0)
		   res = res + argDec;
		 else 
		   res = res -argDec;
		 if(dataIn[0]==0)
		   res = res + 1;
		 else 
		   res = res -1;
		 res = res % 7;
		 dOut = res;
          fRam[ramAddress] = dOut;
		 end
	   else if(op==1) //derivative
	    begin
		  if(dataIn[4]==0)
		   res = res + 4*argDec*argDec*argDec;
		  else 
		   res = res -4*argDec*argDec*argDec;
		  if(dataIn[3]==0)
		   res = res + 3*argDec*argDec;
		  else 
		   res = res -3*argDec*argDec;
		  if(dataIn[2]==0)
		   res = res + 2*argDec;
		  else 
		   res = res -2*argDec;
		  if(dataIn[1]==0)
		   res = res + 1;
		  else 
		   res = res - 1;
          if(res<0)
            begin
              dOut = -res;
              dOut[8] = ~dOut[8];
            end
		  else dOut = res;
          fRam[ramAddress] = dOut;
		 end
		
	  end//write mode
	
   end
  
  
  
endmodule


module FUNCMEMORY(input mode, input [6:0] memInput, input CLK, output wire [8:0] result);
	/*Don't edit this module*/
	wire [4:0]  romData;

	FUNCROM RO(memInput[6:3], romData);
	FUNCRAM RA(mode, memInput[6:3], romData, memInput[2],memInput[1:0], CLK, result);

endmodule