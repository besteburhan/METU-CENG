`timescale 1ns / 1ps 
module lab3_2(
			input[4:0] word,
			input CLK, 
			input selection,
			input mode,
			output reg [7:0] hipsterians1,
			output reg [7:0] hipsterians0,
			output reg [7:0] nerdians1,
			output reg [7:0] nerdians0,
			output reg warning
    );

	initial begin
		hipsterians0=0;
		nerdians0=0;
		hipsterians1=0;
		nerdians1=0;
		warning=0;
	end
   //Modify the lines below to implement your design .
   reg bool;
	initial bool =0;
   
	always @ (posedge (CLK))
	begin
		if(word[0]==word[1])
		begin 
			if(word[0]== selection)
				bool=1;
		end
		else if(word[1]==word[2])
		begin 
			if(word[1]== selection)
				bool=1;
		end
		else if(word[2]==word[3])
		begin 
			if(word[2]== selection)
				bool=1;
		end
		else if(word[3]==word[4])
		begin 
			if(word[3]== selection)
				bool=1;
		end
		
		if (selection==0)
		begin
			if (bool==0) 
			begin
				warning=1;
			end
			else if (bool==1 && mode==0) 
			begin
				if (hipsterians1==0 && hipsterians0==0) 
				begin
					hipsterians1=hipsterians1;
					hipsterians0=hipsterians0;
				end
				else if (hipsterians1!=0 && hipsterians0==0) 
				begin
					hipsterians1=hipsterians1-1;
					hipsterians0=9;
				end
				else
				begin 
					hipsterians1=hipsterians1;
					hipsterians0=hipsterians0-1;
				end
			end
			else if(bool==1 && mode==1)
			begin
				if (hipsterians1==2) 
				begin
					hipsterians1=0;
					hipsterians0=0;
				end
				else if (hipsterians0==9) 
				begin
					hipsterians1 = hipsterians1+1;
					hipsterians0=0;
				end
				else 
				begin 
					hipsterians1=hipsterians1;
					hipsterians0=hipsterians0+1;
				end
			end
		end
		else if(selection==1)
		begin
			if (bool==0) 
			begin
				warning=1;
			end
			else if (bool==1 && mode==0) 
			begin
				if (nerdians1==0 && nerdians0==0) 
				begin
					nerdians1=nerdians1;
					nerdians0=nerdians0;
				end
				else if (nerdians1!=0 && nerdians0==0) 
				begin
					nerdians1=nerdians1-1;
					nerdians0=9;
				end
				else 
				begin 
					nerdians1=nerdians1;
					nerdians0=nerdians0-1;
				end
			end
			else if(bool==1 && mode==1) 
			begin
				if (nerdians1==2) 
				begin
					nerdians1=0;
					nerdians0=0;
				end
				else if (nerdians0==9) 
				begin
					nerdians1=nerdians1+1;
					nerdians0=0;
				end
				else
				begin 
					nerdians1=nerdians1;
					nerdians0=nerdians0+1;
				end
			end
		end  //else selection end
	
					
	
	end
endmodule


