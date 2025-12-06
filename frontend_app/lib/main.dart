import 'package:flutter/material.dart';
import 'package:http/http.dart' as http; // You need to install this

void main() {
  runApp(const AlgoMetricApp());
}

class AlgoMetricApp extends StatelessWidget {
  const AlgoMetricApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'AlgoMetric',
      theme: ThemeData.dark().copyWith(
        scaffoldBackgroundColor: const Color(0xFF1E1E1E),
        primaryColor: const Color(0xFFBB86FC),
      ),
      home: const CodeEditorScreen(),
    );
  }
}

class CodeEditorScreen extends StatefulWidget {
  const CodeEditorScreen({super.key});

  @override
  State<CodeEditorScreen> createState() => _CodeEditorScreenState();
}

class _CodeEditorScreenState extends State<CodeEditorScreen> {
  final TextEditingController _codeController = TextEditingController();
  String _analysisResult = "Ready to analyze...";
  bool _isLoading = false;

  // Function to call your Java Backend
  Future<void> _analyzeCode() async {
    // 1. Validation: Don't send empty code
    if (_codeController.text.isEmpty) {
      setState(() => _analysisResult = "Please paste some code first!");
      return;
    }

    setState(() {
      _isLoading = true;
      _analysisResult = "Sending your code to the Cloud Brain...";
    });

    try {
      // 2. Send POST request with the text from the box
      final queueResponse = await http.post(
        Uri.parse('http://20.235.242.2:8097/api/queue/add'),
        headers: {"Content-Type": "application/json"},
        body: '{"code": "${_codeController.text.replaceAll('\n', '\\n').replaceAll('"', '\\"')}", "language": "java"}'
      );

      if (queueResponse.statusCode == 200) {
        // 3. Trigger Processing
        final processResponse = await http.get(
          Uri.parse('http://20.235.242.2:8097/api/queue/process'),
        );

        setState(() {
          _analysisResult = processResponse.body
              .replaceAll('<h1>', '')
              .replaceAll('</h1>', '\n\n')
              .replaceAll('<p>', '')
              .replaceAll('</p>', '')
              .replaceAll('<br>', '\n');
        });
      } else {
        setState(() {
          _analysisResult = "Error adding to queue: ${queueResponse.statusCode}";
        });
      }
    } catch (e) {
      setState(() {
        _analysisResult = "Connection Error: $e";
      });
    } finally {
      setState(() {
        _isLoading = false;
      });
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: const Text("AlgoMetric Engine")),
      body: Row(
        children: [
          // Left Side: Code Input
          Expanded(
            flex: 1,
            child: Padding(
              padding: const EdgeInsets.all(16.0),
              child: Column(
                children: [
                  Expanded(
                    child: TextField(
                      controller: _codeController,
                      maxLines: null,
                      style: const TextStyle(fontFamily: 'monospace'),
                      decoration: const InputDecoration(
                        border: OutlineInputBorder(),
                        hintText: 'Paste your C/Java/Python code here...',
                        filled: true,
                        fillColor: Color(0xFF2D2D2D),
                      ),
                    ),
                  ),
                  const SizedBox(height: 16),
                  ElevatedButton.icon(
                    onPressed: _isLoading ? null : _analyzeCode,
                    icon: const Icon(Icons.play_arrow),
                    label: const Text("OPTIMIZE CODE"),
                    style: ElevatedButton.styleFrom(
                      padding: const EdgeInsets.symmetric(horizontal: 32, vertical: 20),
                      backgroundColor: const Color(0xFFBB86FC),
                      foregroundColor: Colors.black,
                    ),
                  ),
                ],
              ),
            ),
          ),
          // Right Side: Analysis Output
          Expanded(
            flex: 1,
            child: Container(
              margin: const EdgeInsets.all(16.0),
              padding: const EdgeInsets.all(16.0),
              decoration: BoxDecoration(
                color: const Color(0xFF252526),
                border: Border.all(color: Colors.grey.shade800),
                borderRadius: BorderRadius.circular(8),
              ),
              child: SingleChildScrollView(
                child: Text(
                  _analysisResult,
                  style: const TextStyle(fontFamily: 'monospace', fontSize: 14),
                ),
              ),
            ),
          ),
        ],
      ),
    );
  }
}