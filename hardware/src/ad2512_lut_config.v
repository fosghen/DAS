module ad2512_lut_config(
	input[9:0]             lut_index,   //Look-up table address
	output reg[23:0]       lut_data     //reg address reg data
);

always@(*)
begin
	case(lut_index)			  
		10'd  0: lut_data <= {16'h0000 , 8'h80};  //reset
		10'd  1: lut_data <= {16'h0001 , 8'h00};  //bit[7:4] = 0000 bit[3]:sleep ; bit[2]:low power ; bit[1]: power down B ;bit[0] :0
		10'd  2: lut_data <= {16'h0002 , 8'h01};  //90:05  
		10'd  3: lut_data <= {16'h0003 , 8'h04};  //Output mode register
		10'd  4: lut_data <= {16'h0004 , 8'h00};  //Digital signal format register all 0
		default:lut_data <= {16'hffff,8'hff};
	endcase
end


endmodule 