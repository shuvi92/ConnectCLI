#ifndef OPS_H
#define OPS_H

#include "dds_io.h"

class Ops 
{
    private:
        /* User Information DDS IO Variable */
        dds_io<user_information,user_informationSeq,
                user_informationTypeSupport_var, user_informationTypeSupport,
                user_informationDataWriter_var, user_informationDataWriter,
                user_informationDataReader_var, user_informationDataReader>
                dds_user_info =
            dds_io<user_information, user_informationSeq,
                user_informationTypeSupport_var, user_informationTypeSupport,
                user_informationDataWriter_var, user_informationDataWriter,
                user_informationDataReader_var, user_informationDataReader>
                ( (char*) "user_information", true     , true );

        /* Request DDS IO Variable */
        dds_io<request,requestSeq,
                    requestTypeSupport_var, requestTypeSupport,
                    requestDataWriter_var, requestDataWriter,
                    requestDataReader_var, requestDataReader>
                    dds_request =
            dds_io<request, requestSeq,
                    requestTypeSupport_var, requestTypeSupport,
                    requestDataWriter_var, requestDataWriter,
                    requestDataReader_var, requestDataReader>
                    ( (char*) "request", true     , true );

        /* Response DDS IO Variable */
        dds_io<response,responseSeq,
                    responseTypeSupport_var, responseTypeSupport,
                    responseDataWriter_var, responseDataWriter,
                    responseDataReader_var, responseDataReader>
                    dds_response =
            dds_io<response, responseSeq,
                    responseTypeSupport_var, responseTypeSupport,
                    responseDataWriter_var, responseDataWriter,
                    responseDataReader_var, responseDataReader>
                    ( (char*) "response", true     , true );

        /* Private Message DDS IO Variable */
        dds_io<private_message, private_messageSeq,
                    private_messageTypeSupport_var, private_messageTypeSupport,
                    private_messageDataWriter_var, private_messageDataWriter,
                    private_messageDataReader_var, private_messageDataReader>
                    dds_private_message =
            dds_io<private_message, private_messageSeq,
                    private_messageTypeSupport_var, private_messageTypeSupport,
                    private_messageDataWriter_var, private_messageDataWriter,
                    private_messageDataReader_var, private_messageDataReader>
                    ( (char*) "private_message", true     , true );

    public:
        
        dds_io<user_information,user_informationSeq,
                user_informationTypeSupport_var, user_informationTypeSupport,
                user_informationDataWriter_var, user_informationDataWriter,
                user_informationDataReader_var, user_informationDataReader>
            get_dds_user_info();

        dds_io<request,requestSeq,
                requestTypeSupport_var, requestTypeSupport,
                requestDataWriter_var, requestDataWriter,
                requestDataReader_var, requestDataReader>
            get_dds_request();

        dds_io<response,responseSeq,
                responseTypeSupport_var, responseTypeSupport,
                responseDataWriter_var, responseDataWriter,
                responseDataReader_var, responseDataReader>
            get_dds_response();
        
        dds_io<private_message,private_messageSeq,
                private_messageTypeSupport_var, private_messageTypeSupport,
                private_messageDataWriter_var, private_messageDataWriter,
                private_messageDataReader_var, private_messageDataReader>
            get_dds_pm();
};

#endif