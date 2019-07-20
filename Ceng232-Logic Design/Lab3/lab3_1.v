`timescale 1ns / 1ps

module af(
    input a,
    input f,
    input clk,
    output reg q
    );
    
    initial begin
        q = 0;
    end
	always@(posedge clk)
	   if (a==0 & f==0) q=~q;
		else if (a==1 & f==0) q=1;
      else if (a==1 & f==1) q=0;
      else q=q;		
	

endmodule


module ic1500(
    input a0, 
    input f0, 
    input a1, 
    input f1, 
    input clk, 
    output q0, 
    output q1, 
    output y
    );
    af s0(a0,f0,clk,q0);
	 af s1(a1,f1,clk,q1);
	assign y=q0^q1;
endmodule
