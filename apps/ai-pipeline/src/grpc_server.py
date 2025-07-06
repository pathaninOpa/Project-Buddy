import grpc
from concurrent import futures
import logging
from protos import speech_service_pb2
from protos import speech_service_pb2_grpc
from STS.pipeline import RUN

class SpeechServiceServicer(speech_service_pb2_grpc.SpeechServiceServicer):
    def __init__(self):
        self.pipeline = RUN()

    def ProcessSpeech(self, request, context):
        try:
            response_audio = self.pipeline.pipeline(request.audio_data)
            
            return speech_service_pb2.AudioResponse(
                audio_data=bytes(response_audio),
                transcribed_text="",
                llm_response="" 
            )
        except Exception as e:
            logging.error(f"Error processing speech: {str(e)}")
            context.set_code(grpc.StatusCode.INTERNAL)
            context.set_details(f'Error processing speech: {str(e)}')
            return speech_service_pb2.AudioResponse()

def serve():
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
    speech_service_pb2_grpc.add_SpeechServiceServicer_to_server(
        SpeechServiceServicer(), server
    )
    server.add_insecure_port('[::]:50051')
    server.start()
    print("gRPC server started on port 50051")
    server.wait_for_termination()

if __name__ == '__main__':
    logging.basicConfig(level=logging.INFO)
    serve()
