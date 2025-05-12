module pulse_generator #(
    parameter ADC_PERIOD = 1  // Период (в наносекундах или тиках), используется как шаг уменьшения
)(
    input  wire        clk_adc_i,
    input  wire        reset_i,

    input  wire        signal_start_i,        // Старт по фронту
    input  wire [31:0] width_signal_i,        // Ширина импульса

    output reg         pulse_o                // Импульс на выходе
);

    localparam
        IDLE       = 1'b0,
        PULSE_HIGH = 1'b1;

    reg        state;
    reg [31:0] counter;

    // Для детектирования переднего фронта
    reg signal_start_prev;

    always @(posedge clk_adc_i) begin
        if (~reset_i) begin
            state <= IDLE;
            counter <= 0;
            pulse_o <= 0;
            signal_start_prev <= 0;
        end else begin
            signal_start_prev <= signal_start_i; // обновляем предыдущее значение

            case (state)
                IDLE: begin
                    pulse_o <= 0;
                    // Детектируем передний фронт старта
                    if (signal_start_i && !signal_start_prev) begin
                        counter <= width_signal_i;
                        pulse_o <= 1;
                        state <= PULSE_HIGH;
                    end
                end

                PULSE_HIGH: begin
                    if (counter > ADC_PERIOD) begin
                        counter <= counter - ADC_PERIOD;
                    end else begin
                        pulse_o <= 0;
                        state <= IDLE;
                    end
                end
            endcase
        end
    end

endmodule