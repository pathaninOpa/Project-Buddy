from grpc_tools import protoc
import os

def generate_proto():
    proto_file = "speech_service.proto"
    proto_path = os.path.join("src", "protos")
    
    protoc.main([
        "",
        f"-I{proto_path}",
        f"--python_out={proto_path}",
        f"--grpc_python_out={proto_path}",
        os.path.join(proto_path, proto_file)
    ])

if __name__ == "__main__":
    generate_proto()
    print("Generated gRPC code successfully")
