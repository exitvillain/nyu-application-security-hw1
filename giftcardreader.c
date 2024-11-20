/*
 * Gift Card Reading Application
 * Comments adjusted by: Daniel Gray
 * Assignment: NYU Application Security Homework
 * Maintainer: Daniel Gray
 * Date: [Your Date Here]
 */

#include "giftcard.h"
#include <stdio.h>
#include <string.h>

// Interpreter for the custom THX-1138 assembly
void run_program(char *message, unsigned char *instructions) {
    unsigned char registers[16] = {0};
    char *msg_ptr = message;
    unsigned char *inst_ptr = instructions;
    int zero_flag = 0;
    int step_count = 0;

    while (1) {
        step_count++;
        unsigned char opcode = *inst_ptr;
        unsigned char arg1 = *(inst_ptr + 1);
        unsigned char arg2 = *(inst_ptr + 2);

        switch (opcode) {
            case 0x00: // NOP
                break;
            case 0x01: // LOAD message character into register
                if (arg1 < 16) registers[arg1] = *msg_ptr;
                break;
            case 0x02: // STORE register value into message
                if (arg1 < 16) *msg_ptr = registers[arg1];
                break;
            case 0x03: // MOVE pointer in the message
                msg_ptr += (char)arg1;
                break;
            case 0x04: // SET register value
                if (arg2 < 16) registers[arg2] = arg1;
                break;
            case 0x05: // XOR registers
                if (arg1 < 16 && arg2 < 16) {
                    registers[arg1] ^= registers[arg2];
                    zero_flag = (registers[arg1] == 0);
                }
                break;
            case 0x06: // ADD registers
                if (arg1 < 16 && arg2 < 16) {
                    registers[arg1] += registers[arg2];
                    zero_flag = (registers[arg1] == 0);
                }
                break;
            case 0x07: // PRINT message
                puts(message);
                break;
            case 0x08: // HALT program
                goto end_program;
            case 0x09: // JUMP forward
                inst_ptr += (char)arg1;
                break;
            case 0x10: // CONDITIONAL JUMP forward
                if (zero_flag) inst_ptr += (char)arg1;
                break;
            default: // Unhandled opcode
                break;
        }
        inst_ptr += 3;
        if (inst_ptr > instructions + 256) break; // Prevent overruns
    }
end_program:
    return;
}

void display_card_info(struct this_gift_card *card) {
    struct gift_card_data *data = card->gift_card_data;
    printf("   Merchant ID: %32.32s\n", data->merchant_id);
    printf("   Customer ID: %32.32s\n", data->customer_id);
    printf("   Record Count: %d\n", data->number_of_gift_card_records);

    for (int i = 0; i < data->number_of_gift_card_records; i++) {
        struct gift_card_record_data *record = (struct gift_card_record_data *)data->gift_card_record_data[i];
        if (record->type_of_record == 1) { // Amount Change
            printf("      Record Type: Amount Change\n");
            struct gift_card_amount_change *amount_record = record->actual_record;
            printf("      Amount Added: %d\n", amount_record->amount_added);
            if (amount_record->amount_added > 0) {
                printf("      Signature: %32.32s\n", amount_record->actual_signature);
            }
        } else if (record->type_of_record == 2) { // Message
            printf("      Record Type: Message\n");
            printf("      Message: %s\n", (char *)record->actual_record);
        } else if (record->type_of_record == 3) { // Animated Message
            struct gift_card_program *prog = record->actual_record;
            printf("      Record Type: Animated Message\n");
            printf("      Message: %s\n", prog->message);
            printf("      [Executing Embedded Program]\n");
            run_program(prog->message, prog->program);
        }
    }
    printf("  Total Value: %d\n\n", calculate_card_balance(card));
}

void output_card_json(struct this_gift_card *card) {
    struct gift_card_data *data = card->gift_card_data;
    printf("{\n");
    printf("  \"merchant_id\": \"%32.32s\",\n", data->merchant_id);
    printf("  \"customer_id\": \"%32.32s\",\n", data->customer_id);
    printf("  \"total_value\": %d,\n", calculate_card_balance(card));
    printf("  \"records\": [\n");

    for (int i = 0; i < data->number_of_gift_card_records; i++) {
        struct gift_card_record_data *record = (struct gift_card_record_data *)data->gift_card_record_data[i];
        printf("    {\n");
        if (record->type_of_record == 1) { // Amount Change
            struct gift_card_amount_change *amount_record = record->actual_record;
            printf("      \"record_type\": \"amount_change\",\n");
            printf("      \"amount_added\": %d,\n", amount_record->amount_added);
            if (amount_record->amount_added > 0) {
                printf("      \"signature\": \"%32.32s\"\n", amount_record->actual_signature);
            }
        } else if (record->type_of_record == 2) { // Message
            printf("      \"record_type\": \"message\",\n");
            printf("      \"message\": \"%s\"\n", (char *)record->actual_record);
        } else if (record->type_of_record == 3) { // Animated Message
            struct gift_card_program *prog = record->actual_record;
            printf("      \"record_type\": \"animated_message\",\n");
            printf("      \"message\": \"%s\",\n", prog->message);
        }
        printf("    }%s\n", (i < data->number_of_gift_card_records - 1) ? "," : "");
    }
    printf("  ]\n");
    printf("}\n");
}

int calculate_card_balance(struct this_gift_card *card) {
    struct gift_card_data *data = card->gift_card_data;
    int total_balance = 0;

    for (int i = 0; i < data->number_of_gift_card_records; i++) {
        struct gift_card_record_data *record = (struct gift_card_record_data *)data->gift_card_record_data[i];
        if (record->type_of_record == 1) {
            struct gift_card_amount_change *amount_record = record->actual_record;
            total_balance += amount_record->amount_added;
        }
    }
    return total_balance;
}

struct this_gift_card *read_card_file(FILE *file) {
    struct this_gift_card *card = malloc(sizeof(struct this_gift_card));
    fread(&card->num_bytes, sizeof(int), 1, file);

    void *data_ptr = malloc(card->num_bytes);
    fread(data_ptr, card->num_bytes, 1, file);

    card->gift_card_data = parse_gift_card_data(data_ptr);
    return card;
}

int main(int argc, char **argv) {
    FILE *file = fopen(argv[2], "r");
    struct this_gift_card *card = read_card_file(file);

    if (argv[1][0] == '1') {
        display_card_info(card);
    } else if (argv[1][0] == '2') {
        output_card_json(card);
    }

    return 0;
}
