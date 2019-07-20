`timescale 1ns / 1ps

module lab4_2(
	input[3:0] userID, 
	input CLK, 
	input team,
	input [1:0] mode,
	output reg  [7:0] numPlyLP,
	output reg  [7:0] numPlyCF,
	output reg  teamWng,
	output reg capWngLP,
	output reg capWngCF,
	output reg findRes,
	output reg [3:0] listOut,
	output reg  listMode
    );
//Write your code below
reg [3:0] lPower [4:0];
reg [3:0] cForce [4:0];

initial begin
  numPlyLP =0;
  numPlyCF =0;
  teamWng =0;
  capWngLP =1;
  capWngCF =1;
  findRes =0;
  listMode =0;
  lPower[4] = 4'b0000;
  lPower[3] = 4'b0000;  
  lPower[2] = 4'b0000;
  lPower[1] = 4'b0000;
  lPower[0] = 4'b0000;
  cForce[4] = 4'b0000;
  cForce[3] = 4'b0000;
  cForce[2] = 4'b0000;
  cForce[1] = 4'b0000;
  cForce[0] = 4'b0000;
  
end 
always@(posedge CLK)
begin 
  teamWng=0;
  findRes=0;
  listMode =0;
  //listOut ne olmalı?? no output?
  if(mode==01)
   begin
	  if(userID[3] != team)
	    teamWng = 1;
	  else if (team==0)
	   begin
		  if((lPower[4]!= userID) && (lPower[3] != userID )&& (lPower[2] != userID) && (lPower[1] != userID) && (lPower[0] != userID))
		   begin
			   if(capWngLP ==1 && lPower[4]!=4'b0000)
			     teamWng = 1;
			   else if(capWngLP ==1 && lPower[4]==4'b0000)
			    begin
			     capWngLP =0;
				  numPlyLP = numPlyLP +1;
				  lPower[4]=userID;//??olcak mı
				 end
				else
				 begin
				   if(lPower[4] == 4'b0000)
					  lPower[4] = userID;
					else if(lPower[3] == 4'b0000)
					  lPower[3] = userID;
					else if(lPower[2] == 4'b0000)
					  lPower[2] = userID;
					else if(lPower[1] == 4'b0000)
					  lPower[1] = userID;
					else if(lPower[0] == 4'b0000)
					  lPower[0] = userID;
					numPlyLP = numPlyLP+1;
					if(numPlyLP ==5) 
					  capWngLP =1;		
				 end
			end
		  else;
		end
		
		else if (team==1)
	   begin
		  if((cForce[4]!= userID) && (cForce[3] != userID )&& (cForce[2] != userID) && (cForce[1] != userID) && (cForce[0] != userID))
		   begin
			   if(capWngCF ==1 && cForce[4]!=4'b0000)
			     teamWng = 1;
			   else if(capWngCF ==1 && cForce[4]==4'b0000)
			    begin
			     capWngCF =0;
				  numPlyCF = numPlyCF+1;
				  cForce[4]=userID;//?
				 end
				else
				 begin
				   if(cForce[4] == 4'b0000)
					  cForce[4] = userID;
					else if(cForce[3] == 4'b0000)
					  cForce[3] = userID;
					else if(cForce[2] == 4'b0000)
					  cForce[2] = userID;
					else if(cForce[1] == 4'b0000)
					  cForce[1] = userID;
					else if(cForce[0] == 4'b0000)
					  cForce[0] = userID;
					numPlyCF = numPlyCF+1;
					if(numPlyCF ==5) 
					  capWngCF =1;		
				 end
			end
		  else;
		end
	
	
	end // login mode son
  else if(mode == 00)
   begin
	  if(team == 0 && numPlyLP !=0)
	   begin
		   if(lPower[4] == userID)
			 begin
			   lPower[4] = 4'b0000;
				numPlyLP = numPlyLP -1;
			 end
			else if(lPower[3] == userID)
			 begin
			   lPower[3] = 4'b0000;
				numPlyLP = numPlyLP -1;
			 end
			else if(lPower[2] == userID)
			 begin
			   lPower[2] = 4'b0000;
				numPlyLP = numPlyLP -1;
			 end
			else if(lPower[1] == userID)
			 begin
			   lPower[1] = 4'b0000;
				numPlyLP = numPlyLP -1;
			 end
			else if(lPower[0] == userID)
			 begin
			   lPower[0] = 4'b0000;
				numPlyLP = numPlyLP -1;
			 end
		   if (numPlyLP == 0)
			  capWngLP = 1;
		end
		
	  if(team == 1 && numPlyCF !=0)
	   begin
		   if(cForce[4] == userID)
			 begin
			   cForce[4] = 4'b0000;
				numPlyCF = numPlyCF -1;
			 end
			else if(cForce[3] == userID)
			 begin
			   cForce[3] = 4'b0000;
				numPlyCF = numPlyCF -1;
			 end
			else if(cForce[2] == userID)
			 begin
			   cForce[2] = 4'b0000;
				numPlyCF = numPlyCF -1;
			 end
			else if(cForce[1] == userID)
			 begin
			   cForce[1] = 4'b0000;
				numPlyCF = numPlyCF -1;
			 end
			else if(cForce[0] == userID)
			 begin
			   cForce[0] = 4'b0000;
				numPlyCF = numPlyCF -1;
			 end
		   if (numPlyCF == 0)
			  capWngCF = 1;
		end
	
	end // logout mode sonu
  
  else if(mode==10)
   begin
	  if(team ==0 && numPlyLP !=0)
	   begin
	     if((lPower[4] == userID) || (lPower[3] == userID) || (lPower[2] == userID) || (lPower[1] == userID) || (lPower[0] == userID))
	       findRes = 1 ;
		end 
	  else if(team ==1 && numPlyCF !=0)
	   begin
	     if((cForce[4] == userID) || (cForce[3] == userID) || (cForce[2] == userID) || (cForce[1] == userID) || (cForce[0] == userID))
	       findRes = 1 ;
		end 
	
	end // find mode sonu
  else if (mode == 11)
   begin
	 listMode=1;
	 if(team == 0)
	  begin
	   if(listOut == lPower[4])
		 listOut = lPower[3];
		else if(listOut == lPower[3])
		 listOut = lPower[2];
		else if(listOut == lPower[2])
		 listOut = lPower[1];
		else if(listOut == lPower[1])
		 listOut = lPower[0];
		else if(listOut == lPower[0])
		 listOut = lPower[4];
		else // no output
		 listOut = lPower[4];
	  end
	  else if(team == 1)
	  begin
	   if(listOut == cForce[4])
		 listOut = cForce[3];
		else if(listOut == cForce[3])
		 listOut = cForce[2];
		else if(listOut == cForce[2])
		 listOut = cForce[1];
		else if(listOut == cForce[1])
		 listOut = cForce[0];
		else if(listOut == cForce[0])
		 listOut = cForce[4];
		else // no output
		 listOut = cForce[4];
	  end
	end//list mode sonu



end

endmodule
