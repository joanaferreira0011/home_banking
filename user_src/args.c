#include "args.h"
#include "sope.h"

#define USERID_INDEX 1
#define PASSWORD_INDEX 2
#define OPDELAY_INDEX 3
#define OPCODE_INDEX 4
#define OPARGS_INDEX 5

/* Extracts positive number / zero from string and returns it, will return -1 if an error occurs */
int non_neg_atoi(char *str) {
    return -1;
}

/* Separates the arguments from an argument string */
char **separate_args(char *arg_str) {
    return 0;
}

int process_arguments(int argc, char **argv, tlv_request_t *buffer) {
    if (argc != EXPECTED_ARGC || argv == NULL || buffer == NULL) {
        return 1;
    }
    
    int userid;
    if ((userid = non_neg_atoi(argv[USERID_INDEX])) == -1) {
    	return 1;
    }
    
    int opdelay;
    if ((opdelay = non_neg_atoi(argv[OPDELAY_INDEX]) == -1)) {
    	return 1;
    }
    
    op_type_t opcode = non_neg_atoi(argv[OPCODE_INDEX]);
    char **opargs = separate_args(argv[OPARGS_INDEX]);
    switch (opcode) {
    	case OP_CREATE_ACCOUNT:
            
    		break;
    	case OP_BALANCE:
    		break;
    	case OP_TRANSFER:
    		break;
    	case OP_SHUTDOWN:
    		break;
    	default:
    		break;
    }

    return 0;
}



