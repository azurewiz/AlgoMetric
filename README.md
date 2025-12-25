
# AlgoMetric: Distributed Code Complexity Engine 🚀

![Status](https://img.shields.io/badge/Status-Live-brightgreen) ![Security](https://img.shields.io/badge/Security-OWASP%20Hardened-blue) ![Stack](https://img.shields.io/badge/Stack-Java%20%7C%20Python%20%7C%20Flutter-orange) ![Cloud](https://img.shields.io/badge/Cloud-Azure%20%2B%20Colab-blueviolet)

**AlgoMetric** is a high-performance, distributed system designed to analyze code complexity (Big O Notation) in real-time. It features a unique **hybrid-cloud architecture** that bridges a secure enterprise Java Backend (Azure) with a dynamic Python Analysis Engine (Google Colab) using automated secure tunneling.

---

## 🏗 System Architecture

The system decouples the **API Gateway** from the **Computation Layer**, allowing for independent scaling and cost optimization.

```mermaid
graph LR
    User[Flutter Web App] -- HTTPS JSON --> Backend[Java Spring Boot\n(Azure VM)]
    Backend -- Async Job Queue --> DB[(PostgreSQL)]
    Backend -- Secure Tunnel (Ngrok) --> Engine[Python Analysis Brain\n(Google Colab)]
    Engine -- Metrics & AST --> Backend
    Backend -- Real-time Response --> User
