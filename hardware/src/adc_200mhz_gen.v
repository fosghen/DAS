`timescale 1ns / 1ps

module adc_200mhz_gen(
    input clk_i,
    input reset_i,
    
    output [13:0] data_o
    );
    
    reg [13:0] data_reg;
    reg [13:0] counter;
    
    assign data_o = data_reg;
    
    always @(posedge clk_i) begin
        if (~reset_i) begin
            counter <= 0;
            data_reg <= 0;
        end else begin
            counter <= counter + 1;
            data_reg <= counter;
        end 
    end
endmodule
