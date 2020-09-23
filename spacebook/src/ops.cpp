#include "ops.h"

/* Get User Information DDS IO Variable */
dds_io<user_information,user_informationSeq,
        user_informationTypeSupport_var, user_informationTypeSupport,
        user_informationDataWriter_var, user_informationDataWriter,
        user_informationDataReader_var, user_informationDataReader>
    Ops::get_dds_user_info()
{
    return dds_user_info;
}

/* Get Request DDS IO Variable */
dds_io<request,requestSeq,
        requestTypeSupport_var, requestTypeSupport,
        requestDataWriter_var, requestDataWriter,
        requestDataReader_var, requestDataReader>
    Ops::get_dds_request()
{
    return dds_request;
}

/* Get Response DDS IO Variable */
dds_io<response,responseSeq,
        responseTypeSupport_var, responseTypeSupport,
        responseDataWriter_var, responseDataWriter,
        responseDataReader_var, responseDataReader>
    Ops::get_dds_response()
{
    return dds_response;
}

/* Get Private Message DDS IO Variable */
dds_io<private_message,private_messageSeq,
        private_messageTypeSupport_var, private_messageTypeSupport,
        private_messageDataWriter_var, private_messageDataWriter,
        private_messageDataReader_var, private_messageDataReader>
    Ops::get_dds_pm()
{
    return dds_private_message;
}