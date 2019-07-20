`timescale 1ns / 1ps

module Elevator(input CLK,
					input [1:0] mode,
					input [3:0] request,
					output reg[3:0] currentFloor1,
					output reg[3:0] currentFloor2,
					output reg doorOpen1, 
					output reg doorOpen2, 
					output reg [3:0] listingLeds,
					output reg listBusy,
					output reg state);

//Write your code below
reg [0:3] task_pool [0:7];
integer count;
integer i;
integer listIndex;
integer currentTask1;
integer currentTask2;
integer j;
initial begin
	currentFloor1[3:0] = 1;
	currentFloor2[3:0] = 1;
	doorOpen1=1;
	doorOpen2=1;
	listingLeds=4'b0000;
	listBusy = 0;
	state = 0;
	
	task_pool[0]=4'b0000;
	task_pool[1]=4'b0000;
	task_pool[2]=4'b0000;
	task_pool[3]=4'b0000;
	task_pool[4]=4'b0000;
	task_pool[5]=4'b0000;
	task_pool[6]=4'b0000;
	task_pool[7]=4'b0000;
	count=0;
	i=0;
	listIndex = 0;
	j=9;

end

always@(posedge CLK)
begin
	
	if(mode==2'b00 && state ==0 && listBusy!=1) begin
		
		if(count==8);
		else begin 
			j=9;
			for(i=0;i<8;i=i+1)begin 
				if(task_pool[i]==request) begin
					j=1;
				end
			end

			if(j==9) begin
				task_pool[count]=request;
				count=count+1;
			end
		end
	end
	
	else if((mode==2'b01 || listBusy==1 )&& state==0) begin
		listBusy=1;
		if(task_pool[listIndex]==4'b0000 || listIndex==8) begin
			listingLeds=4'b0000;
			listIndex =0;
			listBusy =0;
		end
		else begin
			
			listingLeds=task_pool[listIndex];
			listIndex = listIndex+1;
			
		end
	end
	
	else if((mode==2'b10 && state==0) && listBusy!=1) begin
		j=9;
		for(i=0;i<8;i=i+1) begin
			if(task_pool[i]==request) begin
				j=i;
			end
		end
		if(j!=9) begin
			
			for(i=0;i<8;i=i+1) begin
					if(j==7) begin
						task_pool[j]=4'b0000;
						count=count-1;
						j=9;
					end
					else if(j==i)begin
					task_pool[j]=task_pool[j+1];
					j=j+1;
					end
			end

		end
	end
	else if(listBusy!=1) begin
		state=1;
		if(count==0 && doorOpen1==1 && doorOpen2==1 ) state=0;

		else if(doorOpen1==1 && doorOpen2==0) begin
			currentTask1=task_pool[0];
			if(currentTask1 != 4'b0000)begin
				j=0;
				for(i=0;i<8;i=i+1) begin
						if(j==7) begin
							task_pool[j]=4'b0000;
							count=count-1;
							j=9;
						end
						else if(j==i)begin
							task_pool[j]=task_pool[j+1];
							j=j+1;
						end
				end
			
				if(currentTask1 != currentFloor1) begin
					if(currentTask1 < currentFloor1) begin
						currentFloor1=currentFloor1-1;
						if(currentTask1 != currentFloor1) doorOpen1=0;
					end
					else begin
						currentFloor1=currentFloor1+1;
						if(currentTask1 != currentFloor1) doorOpen1=0;
					end
				end
			end	
				if(currentTask2 < currentFloor2) begin
						currentFloor2=currentFloor2-1;
						if(currentTask2 == currentFloor2) doorOpen2=1;
				end
				else begin
					currentFloor2=currentFloor2+1;
					if(currentTask2 == currentFloor2) doorOpen2=1;
				end
		end
			
		
		
		else if(doorOpen1 ==0 && doorOpen2==1)begin
				currentTask2=task_pool[0];
				if(currentTask2 != 4'b0000)begin
					j=0;
					for(i=0;i<8;i=i+1) begin
						if(j==7) begin
							task_pool[j]=4'b0000;
							count=count-1;
							j=9;
						end
						else if(j==i)begin
							task_pool[j]=task_pool[j+1];
							j=j+1;
						end
					end
					if(currentTask2 != currentFloor2) begin
						if(currentTask2 < currentFloor2) begin
							currentFloor2=currentFloor2-1;
							if(currentTask2 != currentFloor2) doorOpen2=0;
						end
						else begin
							currentFloor2=currentFloor2+1;
							if(currentTask2 != currentFloor2) doorOpen2=0;
						end
					end
				end
				if(currentTask1 < currentFloor1) begin
					currentFloor1=currentFloor1-1;
					if(currentTask1 == currentFloor1) doorOpen1=1;
				end
				else begin
					currentFloor1=currentFloor1+1;
					if(currentTask1 == currentFloor1) doorOpen1=1;
				end
	
		end
		else if(doorOpen1==0 && doorOpen2==0) begin
			if(currentTask1 < currentFloor1) begin
				currentFloor1=currentFloor1-1;
				if(currentTask1 == currentFloor1) doorOpen1=1;
			end
			else begin
				currentFloor1=currentFloor1+1;
				if(currentTask1 == currentFloor1) doorOpen1=1;
			end
				
			if(currentTask2 < currentFloor2) begin
					currentFloor2=currentFloor2-1;
					if(currentTask2 == currentFloor2) doorOpen2=1;
			end
			else begin
				currentFloor2=currentFloor2+1;
				if(currentTask2 == currentFloor2) doorOpen2=1;
			end
		
		end
		
		else begin
			
			currentTask1=task_pool[0];
			j=0;
				for(i=0;i<8;i=i+1) begin
						if(j==7) begin
							task_pool[j]=4'b0000;
							count=count-1;
							j=9;
						end
						else if(j==i)begin
							task_pool[j]=task_pool[j+1];
							j=j+1;
						end
				end
			if(currentTask1 != currentFloor1) begin
				if(currentTask1 < currentFloor1) begin
					currentFloor1=currentFloor1-1;
					if(currentTask1 != currentFloor1) doorOpen1=0;
				end
				else begin
					currentFloor1=currentFloor1+1;
					if(currentTask1 != currentFloor1) doorOpen1=0;
				end
			end
			
			if(count!=0) begin
				currentTask2=task_pool[0];
				j=0;
				for(i=0;i<8;i=i+1) begin
						if(j==7) begin
							task_pool[j]=4'b0000;
							count=count-1;
							j=9;
						end
						else if(j==i)begin
							task_pool[j]=task_pool[j+1];
							j=j+1;
						end
				end
				if(currentTask2 != currentFloor2) begin
					if(currentTask2 < currentFloor2) begin
						currentFloor2=currentFloor2-1;
						if(currentTask2 != currentFloor2) doorOpen2=0;
					end
					else begin
						currentFloor2=currentFloor2+1;
						if(currentTask2 != currentFloor2) doorOpen2=0;
					end
				end
			end
			
		end
		
	end	

	
	
end

endmodule