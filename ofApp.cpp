#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetLineWidth(2);
	ofEnableDepthTest();

	this->frame_mesh.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->face_mesh.clear();
	this->frame_mesh.clear();

	auto R = 100.f;
	auto u = 0.f;
	auto u_step = 10.f;
	auto v_step = 5.f;

	ofColor color_1, color_2;
	for (int i = 0; i < 36; i++) {

		auto r = ofMap(ofNoise(ofRandom(1000), ofGetFrameNum() * 0.005), 0, 1, 10, 30);

		float angle_z = (i * 5) * DEG_TO_RAD;
		auto rotation_z = glm::rotate(glm::mat4(), angle_z, glm::vec3(0, 0, 1));

		color_1.setHsb(ofMap(i, 0, 36, 0, 255), 200, 255);
		color_2.setHsb(ofMap((i + 1) % 36, 0, 36, 0, 255), 200, 255);		

		for (auto v = 0; v < 360; v += v_step) {
		
			vector<glm::vec3> vertices{ 
				glm::vec4(this->make_point(R, r, u, v), 0) * rotation_z,
				glm::vec4(this->make_point(R, r, u + u_step, v + v_step), 0) * rotation_z,
				glm::vec4(this->make_point(R, r, u, v + v_step), 0) * rotation_z,
				glm::vec4(this->make_point(R, r, u - u_step, v), 0) * rotation_z
			};

			this->face_mesh.addVertices(vertices);
			this->frame_mesh.addVertices(vertices);

			auto index = this->face_mesh.getVertices().size() - 1;

			this->face_mesh.addIndex(index - 3); this->face_mesh.addIndex(index - 2); this->face_mesh.addIndex(index - 1);
			this->face_mesh.addIndex(index); this->face_mesh.addIndex(index - 3); this->face_mesh.addIndex(index - 1);

			this->frame_mesh.addIndex(index - 3); this->frame_mesh.addIndex(index - 2);
			this->frame_mesh.addIndex(index); this->frame_mesh.addIndex(index - 1);

			u += u_step;
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();

	ofSetColor(239);
	this->face_mesh.draw();

	ofSetColor(39);
	this->frame_mesh.draw();

	this->cam.end();
}

//--------------------------------------------------------------
glm::vec3 ofApp::make_point(float R, float r, float u, float v) {

	// 数学デッサン教室 描いて楽しむ数学たち　P.31

	u *= DEG_TO_RAD;
	v *= DEG_TO_RAD;

	auto x = (R + r * cos(u)) * cos(v);
	auto y = (R + r * cos(u)) * sin(v);
	auto z = r * sin(u);

	return glm::vec3(x, y, z);
}


//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}