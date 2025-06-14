from llama_cpp import Llama
n_gpu_layers = -1

llm = Llama.from_pretrained(
    repo_id="google/gemma-3-4b-it",
    filename="gemma-3-4b-it.gguf",
    n_gpu_layers = n_gpu_layers
)
while (True):
    # Get user input and format it as a user message
    user_input_text = input("You: ")
    if user_input_text.strip().lower() == "quit":
        print("AI: Goodbye!")
        break
    user_message = {"role": "user", "content": user_input_text}

    response = llm.create_chat_completion(
        messages = [
            {
                "role": "system",
                "content": "You are a helpful AI assistant specializing in Thai language. You can communicate in both Thai and English. Provide clear, concise, and accurate responses."
            },
            user_message # Use the properly formatted user message here
        ]
    )

    print("AI:", response['choices'][0]['message']['content'])