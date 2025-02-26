`timescale 1ns / 1ps

module tb_adc_200mhz_gen;

    // Inputs
    reg clk_i;
    reg reset_i;

    // Outputs
    wire [13:0] data_o;

    // Instantiate the Unit Under Test (UUT)
    adc_200mhz_gen uut (
        .clk_i(clk_i),
        .reset_i(reset_i),
        .data_o(data_o)
    );

    // Clock generation
    always begin
        #2 clk_i = ~clk_i;  // создаем тактовый сигнал с периодом 4ns (200 MHz)
    end

    // Stimulus block
    initial begin
        // Initialize inputs
        clk_i = 0;
        reset_i = 0;

        // Wait for global reset
        #10;
        
        // Apply reset
        reset_i = 1;
        #10;

        // Deassert reset and run the simulation
        reset_i = 0;
        #100;  // ждём 100 ns для наблюдения изменения в данных

        // Check the output and end simulation
        $display("Final value of data_o: %d", data_o);
        #10;
        $stop;  // останавливаем симуляцию
    end

endmodule
