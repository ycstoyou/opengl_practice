#version 330 core

in vec4 vertexColor; // vs�κ��� �Էµ� ���� (���� ������, ���� Ÿ��)
out vec4 FragColor; // ���� ��� ����

void main() {
  FragColor = vertexColor;
}