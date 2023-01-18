module ps2 (
	clk, rst, 
	data_req, data, data_err, data_ack, 
	data_pin, clk_pin
);
	input clk, rst;
	
	input data_req;
	output reg [7:0] data;
	output reg data_err;
	output reg data_ack;
	
	input data_pin, clk_pin;
	
	reg [10:0] incoming_data;
	wire all_bits_received;
	wire transmitting_data = data_ack && data_req;
	
	counter #(12) bit_counter (
		.clk(~clk_pin),
		.rst(rst || transmitting_data),
		.en(1),
		.will_overflow(all_bits_received)
	);
	
	always @(negedge clk_pin) begin
		incoming_data <= { data_pin, incoming_data[10:1] };
	end
	
	task save_data;
		begin
			data <= incoming_data[8:1];
			data_err <= ~^incoming_data[8:1] == incoming_data[9];
			data_ack <= 1'b1;
		end
	endtask
	
	task reset;
		begin
			data <= 8'b0;
			data_err <= 1'b0;
			data_ack <= 1'b0;
		end
	endtask
	
	task on_clock;
		begin
			if(all_bits_received && data_req)
				save_data;
					
			if(transmitting_data)
				data_ack <= 1'b0;
		end
	endtask
	
	always @(posedge clk or posedge rst) begin
		if (rst) reset;
		else on_clock;
	end
endmodule

module counter (
	clk, rst, en,
	value, 
	will_overflow
);
	parameter LIMIT = 1_000;
	parameter START_FROM = 0;
	parameter CONTINUE_FROM = START_FROM;
	
	localparam BITS = $clog2(LIMIT);
	localparam INCREMENT = 1'b1;

	input clk, rst, en;
	
	output reg [BITS-1:0] value = START_FROM;
	output will_overflow;
	
	assign will_overflow = value + INCREMENT == LIMIT;
	
	task reset;
		begin
			value <= START_FROM;
		end
	endtask
	
	task on_clock;
		begin
			if (en)
				value <= will_overflow ? CONTINUE_FROM : value + INCREMENT;
		end
	endtask
	
	always @(posedge clk or posedge rst) begin
		if (rst) reset;
		else on_clock;
	end
endmodule