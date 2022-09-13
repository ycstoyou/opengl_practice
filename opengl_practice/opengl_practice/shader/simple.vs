#version 330 core
layout (location = 0) in vec3 aPos; // 0��° attribute�� ������ ��ġ

out vec4 vertexColor; // fragment shader�� �Ѿ �÷���

void main() {
  gl_Position = vec4(aPos, 1.0); // vec3�� vec4 �����ڿ� ���
  vertexColor = vec4(0.5, 0.5, 0.0, 1.0); // ��ο� �������� ��°�����
}