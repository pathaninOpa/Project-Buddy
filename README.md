# Buddy Project Documentation

## Table of Contents

1. [Project Overview](#project-overview)
2. [Project Structure](#project-structure)
3. [Development Setup](#development-setup)
4. [How to Work on Each Service](#how-to-work-on-each-service)
5. [Running the Services](#running-the-services)
6. [Using Docker](#using-docker)
7. [Environment Variables](#environment-variables)
8. [Adding Dependencies](#adding-dependencies)
9. [Communication Between Services](#communication-between-services)
10. [IDE Setup](#ide-setup)
11. [Using Makefile](#using-makefile)
12. [Troubleshooting](#troubleshooting)

---

## 1. Project Overview

**Buddy** is a multi-service project consisting of three main components:

* **Hardware Services**: Interacts with hardware devices to send and receive data.
* **Mobile Application**: Flutter-based app targeting Android and iOS devices.
* **AI Pipeline**: Handles AI tasks such as NLP and data processing.

This project is organized as a monorepo for easier management.

---

## 2. Project Structure

```
/
├── apps/
│   ├── hardware-services/
│   │   ├── src/
│   │   │   ├── main.py
│   │   │   └── ... (hardware code)
│   │   ├── requirements.txt
│   │   └── ... (auxiliary files)
│   ├── mobile-app/
│   │   ├── src/
│   │   │   ├── lib/
│   │   │   ├── pubspec.yaml
│   │   │   └── ... (Flutter app)
│   │   ├── dockerfile.dev
│   │   └── ... (auxiliary files)
│   └── ai-pipeline/
│       ├── src/
│       │   ├── main.py
│       │   └── ... (AI code)
│       ├── requirements.txt
│       └── ... (auxiliary files)
├── docker-compose.yml
├── makefile
├── .env
├── .vscode/
└── README.md
```

---

## 3. Development Setup

### Prerequisites

* Docker & Docker Compose installed
* Python 3.11+ installed (for local dev on hardware and AI)
* Flutter SDK installed (for local mobile app dev)
* Node.js & pnpm installed (if any JS dependencies, optional)
* For iOS development: macOS with Xcode installed
* For Android development: Android SDK installed

---

## 4. How to Work on Each Service

### Hardware Services

```bash
# Install dependencies
make install-hardware-<package-name>  # Example: make install-hardware-numpy

# Build and run with Docker
make build-hardware  # Build the service
make run-hardware    # Run the service
make stop-hardware   # Stop the service

# For local development (optional)
cd apps/hardware-services
python -m venv venv
source venv/bin/activate     # Linux/macOS
venv\Scripts\activate        # Windows PowerShell
pip install <dependency name>
pip freeze > requirements.txt
python src/main.py
```

### Mobile Application

#### Using Docker (Recommended)

1. Start the development container:
   ```bash
   docker-compose up mobile
   ```

2. Access the container:
   ```bash
   docker exec -it buddy-mobile bash
   ```

3. Inside the container, you can:
   ```bash
   # List available devices
   flutter devices

   # Run on Android device
   flutter run -d android

   # Run on iOS device/simulator
   flutter run -d ios

   # Run in debug mode with hot reload
   flutter run --debug
   ```

4. Development workflow:
   - Your local `apps/mobile-app` directory is mounted to `/app` in the container
   - Changes made on your host machine are immediately reflected in the container
   - Hot reload works automatically - just save your changes
   - Use your preferred IDE on your host machine

5. Building the app:
   ```bash
   # Build Android APK
   flutter build apk

   # Build iOS app
   flutter build ios
   ```

6. Common commands:
   ```bash
   # Check Flutter setup
   flutter doctor

   # Get dependencies
   flutter pub get

   # Clean build
   flutter clean

   # Run tests
   flutter test
   ```

#### Local Development (Alternative)

```bash
cd apps/mobile-app
flutter pub get
flutter run
```

### AI Pipeline

```bash
# Install dependencies
make install-ai-<package-name>  # Example: make install-ai-tensorflow

# Build and run with Docker
make build-ai  # Build the service
make run-ai    # Run the service
make stop-ai   # Stop the service

# For local development (optional)
cd apps/ai-pipeline
python -m venv venv
source venv/bin/activate     # Linux/macOS
venv\Scripts\activate        # Windows PowerShell
pip install -r requirements.txt
python src/main.py
```

---

## 5. Running the Services Using Docker

Build and start all services:

```bash
docker-compose up --build
```
Or
```bash
make build-all
```

Run all service:

```bash
docker-compose up
```
Or
```bash
make run-all
```

Stop all services:

```bash
docker-compose down
```
Or
```bash
make stop-all
```

---

## 6. Using Docker

* Docker images include dependencies for Python and Flutter
* Mobile app container includes Android SDK and iOS development tools
* You can still run apps from CLI for better hardware/emulator integration
* Volume mounts ensure your local changes are reflected in the container

---

## 7. Environment Variables

Define ports and other config in `.env` file at project root:

```env
# AI Service
AI_PORT=xxxx
AI_CONTAINER_PORT=xxxx

# Hardware Service
HARDWARE_PORT=xxxx
HARDWARE_CONTAINER_PORT=xxxx

# Mobile App
ANDROID_HOME=/path/to/android/sdk  # For Android development
```

Update `docker-compose.yml` to use these values with `${VAR_NAME}` syntax.

---

## 8. Adding Dependencies

### Python (Hardware, AI)

```bash
cd apps/<service name>
make install-<ai/hardware>-<dependency name>

or
# optional(manual)
python -m venv venv
source venv/bin/activate     # Linux/macOS
venv\Scripts\activate        # Windows PowerShell
pip install <dependency name>
pip freeze > requirements.txt

then

make build-<ai/hardware>

or
#optional(manual)
docker-compose build <service-name>  # Rebuild to update docker image
docker image prune -f
```

### Flutter (Mobile)

Add dependency to `pubspec.yaml`, then:

```bash
flutter pub get
```

---

## 9. Communication Between Services

* Services are on the same Docker Compose network
* Use service names as hostnames in API calls
* Example: `http://hardware:5000` from AI service to hardware service
* Mobile app can communicate with services using the host machine's IP

---

## 10. IDE Setup

### VSCode Configuration

The project includes VSCode settings in `.vscode/` directory:
* Recommended extensions
* Debug configurations
* Flutter/Dart settings
* Python settings

Install recommended extensions when prompted by VSCode.

---

## 11. Using Makefile

The project includes makefiles at both root and service levels:

### Root Level Makefile
```bash
# Build all docker services
make build-all

# Run all docker services
make run-all

# Stop all docker services
make stop-all
```

### Hardware Service Makefile (apps/hardware-services/makefile)
```bash
# Install dependencies
make install-hardware-<package-name>  # Example: make install-hardware-numpy

# Build and run with Docker
make build-hardware  # Build the service
make run-hardware    # Run the service
make stop-hardware   # Stop the service
```

### AI Pipeline Makefile (apps/ai-pipeline/makefile)
```bash
# Install dependencies
make install-ai-<package-name>  # Example: make install-ai-tensorflow

# Build and run with Docker
make build-ai  # Build the service
make run-ai    # Run the service
make stop-ai   # Stop the service
```

Each makefile is designed to handle its specific service's operations. The root makefile coordinates all services, while service-specific makefiles handle individual service operations.

---

## 12. Troubleshooting

* Clean Docker cache and rebuild if builds fail:
  ```bash
  docker-compose build --no-cache
  ```

* Check Python version compatibility if pip installs fail

* For Flutter issues:
  ```bash
  # Verify device connection
  flutter devices

  # Check Flutter setup
  flutter doctor

  # Clean and rebuild
  flutter clean
  flutter pub get
  ```

* For iOS development:
  - Ensure Xcode is properly installed
  - Accept Xcode license agreements
  - Set up iOS development certificates

* For Android development:
  - Verify Android SDK installation
  - Check USB debugging is enabled
  - Ensure device is authorized
