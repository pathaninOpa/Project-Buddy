import grpc
from concurrent import futures
import logging
from protos import speech_service_pb2
from protos import speech_service_pb2_grpc
from STS.pipeline import RUN
from firebase_fast_logger import ChatLogger
class SpeechServiceServicer(speech_service_pb2_grpc.SpeechServiceServicer):
    def __init__(self):
        self.pipeline = RUN()
        self.logger = ChatLogger()

    def ProcessSpeech(self, request_iterator, context):
        audio_buffer = bytearray()
        current_uid = "unknown_caregiver"
        current_buddy_id = "unknown_buddy"
        try:
            for request in request_iterator:
                audio_buffer.extend(request.audio_data)
                if hasattr(request, 'uid') and request.uid:
                    current_uid = request.uid
                if hasattr(request, 'buddy_id') and request.buddy_id:
                    current_buddy_id = request.buddy_id
            logging.info(f"Received {len(audio_buffer)} bytes of audio data. UID: {current_uid}, Buddy: {current_buddy_id}")
            transcribed_text, response_from_llm, response_audio, trigger_call = self.pipeline.pipeline(bytes(audio_buffer))

            if transcribed_text and response_from_llm:
                self.logger.log_chat(current_uid, current_buddy_id, str(transcribed_text), str(response_from_llm))

            yield speech_service_pb2.AudioResponse(
                audio_data=bytes(response_audio),
                transcribed_text=str(transcribed_text),
                llm_response=str(response_from_llm),
                trigger_call=trigger_call
            )
        except Exception as e:
            logging.error(f"Error processing speech: {str(e)}")
            context.set_code(grpc.StatusCode.INTERNAL)
            context.set_details(f'Error processing speech: {str(e)}')
            return

def serve():
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
    speech_service_pb2_grpc.add_SpeechServiceServicer_to_server(
        SpeechServiceServicer(), server
    )
    try:
        private_key = open("/home/nicestrik/key.pem","rb").read()
        certificate_chain = open("/home/nicestrik/cert.pem","rb").read()
        server_credentials = grpc.ssl_server_credentials(((private_key, certificate_chain,),))
        server.add_secure_port('[::]:50051',server_credentials)
        server.start()
        print("gRPC server started on port 50051")
    except FileNotFoundError:
        print("SSL Keys not found at /home/nicestrik/. Check your paths!")
        return
    server.wait_for_termination()

if __name__ == '__main__':
    logging.basicConfig(level=logging.INFO)
    serve()
