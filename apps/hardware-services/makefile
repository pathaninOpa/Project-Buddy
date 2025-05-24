install-hardware-%:
	python -m venv venv
	.\\venv\\Scripts\\activate && pip install "$(subst install-hardware-,,$@)"
	.\\venv\\Scripts\\activate && pip freeze > requirements.txt

build-hardware:
	docker compose up --build hardware
	docker image prune -f

run-hardware:
	docker compose up hardware

stop-hardware:
	docker compose down hardware

