/* Machine-generated using Migen */
module ft601_proteus_k7_160t(
	inout [31:0] usb_fifo_data,
	inout [3:0] usb_fifo_be,
	input usb_fifo_txe_n,
	input usb_fifo_rxf_n,
	output usb_fifo_wr_n,
	input usb_fifo_rd_n,
	output usb_fifo_oe_n,
	input usb_fifo_wakeup_n,
	input clk100
);

wire sys_clk;
wire sys_rst;
wire por_clk;
reg int_rst = 1'd1;
wire [31:0] data_tri_o;
wire data_tri_oe;
wire [31:0] data_tri_i;
wire [3:0] be_tri_o;
wire be_tri_oe;
wire [3:0] be_tri_i;
wire txe_n_d4;
reg txe_n_d5 = 1'd0;
reg wr_n_d1 = 1'd1;
reg wr_n_d0 = 1'd1;
reg [31:0] data_d0 = 32'd0;
reg [31:0] data_d1 = 32'd0;
reg [3:0] prefetch_counter = 4'd0;
reg [31:0] counter = 32'd0;
(* dont_touch = "true" *) reg multiregimpl0 = 1'd1;
(* dont_touch = "true" *) reg multiregimpl1 = 1'd1;
(* dont_touch = "true" *) reg multiregimpl2 = 1'd1;
(* dont_touch = "true" *) reg multiregimpl3 = 1'd1;

// synthesis translate_off
reg dummy_s;
initial dummy_s <= 1'd0;
// synthesis translate_on

assign sys_clk = clk100;
assign por_clk = clk100;
assign sys_rst = int_rst;
assign be_tri_o = 4'd15;
assign be_tri_oe = 1'd1;
assign data_tri_oe = 1'd1;
assign usb_fifo_oe_n = 1'd1;
assign usb_fifo_wr_n = wr_n_d1;
assign data_tri_o = data_d1;
assign txe_n_d4 = multiregimpl3;

always @(posedge por_clk) begin
	int_rst <= 1'd0;
end

always @(posedge sys_clk) begin
	txe_n_d5 <= txe_n_d4;
	data_d1 <= data_d0;
	wr_n_d1 <= wr_n_d0;
	data_d0 <= counter;
	if (((txe_n_d4 == 1'd0) & (txe_n_d5 == 1'd0))) begin
		wr_n_d0 <= 1'd0;
		if ((prefetch_counter < 3'd6)) begin
			prefetch_counter <= (prefetch_counter + 1'd1);
		end else begin
			prefetch_counter <= prefetch_counter;
		end
		counter <= (counter + 1'd1);
	end else begin
		if ((txe_n_d4 == 1'd1)) begin
			wr_n_d0 <= 1'd1;
			prefetch_counter <= 1'd0;
			counter <= (counter - prefetch_counter);
		end
	end
	if (sys_rst) begin
		txe_n_d5 <= 1'd0;
		wr_n_d1 <= 1'd1;
		wr_n_d0 <= 1'd1;
		data_d0 <= 32'd0;
		data_d1 <= 32'd0;
		prefetch_counter <= 4'd0;
		counter <= 32'd0;
	end
	multiregimpl0 <= usb_fifo_txe_n;
	multiregimpl1 <= multiregimpl0;
	multiregimpl2 <= multiregimpl1;
	multiregimpl3 <= multiregimpl2;
end

assign usb_fifo_data = data_tri_oe ? data_tri_o : 32'bz;
assign data_tri_i = usb_fifo_data;

assign usb_fifo_be = be_tri_oe ? be_tri_o : 4'bz;
assign be_tri_i = usb_fifo_be;

endmodule
