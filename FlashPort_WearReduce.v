// Copyright (C) 1991-2011 Altera Corporation
// Your use of Altera Corporation's design tools, logic functions 
// and other software and tools, and its AMPP partner logic 
// functions, and any output files from any of the foregoing 
// (including device programming or simulation files), and any 
// associated documentation or information are expressly subject 
// to the terms and conditions of the Altera Program License 
// Subscription Agreement, Altera MegaCore Function License 
// Agreement, or other applicable license agreement, including, 
// without limitation, that your use is for the sole purpose of 
// programming logic devices manufactured by Altera and sold by 
// Altera or its authorized distributors.  Please refer to the 
// applicable agreement for further details.

// *****************************************************************************
// This file contains a Verilog test bench template that is freely editable to  
// suit user's needs .Comments are provided in each section to help the user    
// fill out necessary details.                                                  
// *****************************************************************************
// Generated on "04/20/2017 18:57:41"
                                                                                
// Verilog Test Bench template for design : FlashPort_WearReduce
// 
// Simulation tool : ModelSim (Verilog)
// 

`timescale 1 ps/ 1 ps
module FlashPort_WearReduce_vlg_tst();
	// constants                                           
	
	// test vector input registers
	reg Clk;
	reg [1:0] Cmd;
	reg [7:0] treg_FlashIo;
	reg [11:0] InputPt;
	reg Rstn;
	// wires                                               
	wire Ale;
	wire Cle;
	wire FlashCeBar;
	wire [7:0]  FlashIo;
	wire FlashReBar;
	wire FlashWeBar;
	wire [11:0]  OutputPt;

	event Run;
	parameter CmdIdle=2'd0;
	parameter CmdWrite=2'd1;
	parameter CmdRead=2'd2;

	parameter tR=400;
	parameter tRR=1;	
	parameter TRdBsy=tR+tRR;
	
	parameter TWrBsy=11200;	
	parameter TErBsy=32000;	
	
	// assign statements (if any)                          
	assign FlashIo = treg_FlashIo;
	FlashPort_WearReduce i1 (
	// port map - connection between master ports and signals/registers   
		.Ale(Ale),
		.Cle(Cle),
		.Clk(Clk),
		.Cmd(Cmd),
		.FlashCeBar(FlashCeBar),
		.FlashIo(FlashIo),
		.FlashReBar(FlashReBar),
		.FlashWeBar(FlashWeBar),
		.InputPt(InputPt),
		.OutputPt(OutputPt),
		.Rstn(Rstn)
	);
	     
	initial begin
		Clk=1'b0;
		Rstn=1'b0;	
		InputPt=12'd0;	
		Cmd=CmdIdle;
		treg_FlashIo=8'bzzzz_zzzz;		                                                  	                                            
		$display("Running testbench");                       
		end

	initial begin
		forever #31.25 Clk=~Clk;
		end	
	                                                    
	initial begin
		repeat (3) @(posedge Clk);
			Rstn=1'b1;
			->Run;
		end	
		
	always @Run begin
		@(posedge Clk);
		@(posedge Clk);
		@(posedge Clk);
		Cmd=CmdWrite;
		repeat(1) @(posedge Clk);
		Cmd=CmdIdle;
		repeat(15+401) @(posedge Clk);
			
		repeat (3) begin
			treg_FlashIo=8'h001;
				repeat(2) @(posedge Clk);
			treg_FlashIo=8'bzzzz_zzzz;
				repeat (7)begin
					treg_FlashIo=8'h18;
					repeat(2) @(posedge Clk);
					end							
			end
		treg_FlashIo=8'h001;
			repeat(2) @(posedge Clk);			
		treg_FlashIo=8'bzzzz_zzzz;
			repeat(55) @(posedge Clk);				
		$stop;		                                            
		end                                                    
endmodule

