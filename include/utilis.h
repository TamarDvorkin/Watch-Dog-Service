#define ALLOCATE_FAILED(val) NULL == val
#define FREE(val) {free(val); val=NULL;}
#define UNUSED(param) (void)param
#define TRUE 1
#define FALSE 0


#define RETURN_IF_FAIL(is_bad, err_msg, ret_val)\
if(is_bad)                                      \
{                                               \
fputs(err_msg, stderr);                         \
                                                \
return ret_val;                                 \
}


#define PRINT_LINE fprintf(stderr, "%s; err line %d;\n", FILE,LINE);

/*
#define RETURN_IF_MALLOC_FAIL_VOID(is_good, err_msg, parent) 
if (!(is_good))                                             
{
    fputs(err_msg, stderr);
    free(parent);
    
    return;
}


#define RETURN_IF_FAIL_VOID(is_good, err_msg) 
if (!(is_good))                                             
{
    fputs(err_msg, stderr);
    
    return;
}
*/
/* ITS REPLACE:
dhcp = (dhcp_ty *)malloc(sizeof(dhcp_ty));
    if(NULL == dhcp)
    {
        fprintf(stderr, "1Allocation fail!");
        
        return NULL;
    }


IN YOUR CODE:
RETURN_IF_BAD_NESTED(NULL == dhcp->tree, "Allocation fail", -1, dhcp);
*/


