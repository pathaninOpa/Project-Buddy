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

    def ProcessSpeech(self, request, context):
        audio_buffer = bytearray()
        current_uid = "unknown_caregiver"
        current_buddy_id = "unknown_buddy"
        current_reminders_text = ""

        # 1. Attempt to read from Metadata (Headers)
        for key, value in context.invocation_metadata():
            if key == 'uid':
                current_uid = value
            elif key == 'buddy_id':
                current_buddy_id = value
            elif key == 'active_reminders_text':
                current_reminders_text = value

        try:
            for req_chunk in request:
                audio_buffer.extend(req_chunk.audio_data)
                # 2. Fallback/Override from Request Payload (if sent in first chunk)
                if hasattr(req_chunk, 'uid') and req_chunk.uid:
                    current_uid = req_chunk.uid
                if hasattr(req_chunk, 'buddy_id') and req_chunk.buddy_id:
                    current_buddy_id = req_chunk.buddy_id
                if hasattr(req_chunk, 'active_reminders_text') and req_chunk.active_reminders_text:
                    current_reminders_text = req_chunk.active_reminders_text

            logging.info(f"Received {len(audio_buffer)} bytes of audio data. UID: {current_uid}, Buddy: {current_buddy_id}")
            
            if len(audio_buffer) == 0:
                logging.warning(f"Empty audio buffer received from UID: {current_uid}. Skipping processing.")
                return

            # Pass reminders text to pipeline
            transcribed_text, response_from_llm, response_audio, trigger_call = self.pipeline.pipeline(
                bytes(audio_buffer), 
                current_uid, 
                current_buddy_id, 
                current_reminders_text
            )

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
        server_credentials = grpc.ssl_server_credentials([(private_key, certificate_chain)])
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
