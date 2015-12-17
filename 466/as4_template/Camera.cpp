// Allow use of M_PI constant
#define _USE_MATH_DEFINES

#include "Camera.h"
#include "util.h"

Camera::Camera(int camType)
{
    if (camType != PERSPECTIVE && camType != ORTHOGRAPHIC) {
        error_exit(1000, "Invalid arguments");
    }
    type = camType;
    Position.x = 0.0;
    Position.y = 0.0;
    Position.z = 0.0;

    Pitch = M_PI/2;
    Yaw = 0.0;
    v.i = 0; v.j = 1; v.k = 0;
    EnforceVectors();

    ViewWidth = 8.0;
    ViewHeight = 8.0;

    FarPlane = 10.0;
    NearPlane = 2.0;
    ViewPlane = 10.0;
    minDepth = 5.0;

    LookAt();
}

bool Camera::Contains(Vertex v) {
    return (/*v.x <= 5 && v.x >= -5
            && v.y <= 5 && v.y >= -5
            && */v.z <= FarPlane && v.z >= NearPlane);
}

// Move the camera with respect to its viewing vectors
void Camera::Move(float tx, float ty, float tz)
{
    Position.x += tz*n.i + ty*v.i + tx*u.i;
    Position.y += tz*n.j + ty*v.j + tx*u.j;
    Position.z += tz*n.k + ty*v.k + tx*u.k;
    LookAt();
}

// Pan the camera about its local axes
void Camera::Pan(float y, float p)
{
    Yaw -= y;
    Pitch -= p;
    EnforceVectors();
    LookAt();
}

// Scale the viewing plane
void Camera::ScaleView(float s)
{
    ViewWidth = s*ViewWidth;
    ViewHeight = s*ViewHeight;
}

// Move the viewing plane toward/away from the camera
void Camera::MoveView(float d)
{
    if(ViewPlane + d > 1.0)
        ViewPlane = ViewPlane + d;
}

void Camera::MoveNearPlane(float d)
{
    if (NearPlane - d > 1 && NearPlane - d < FarPlane - minDepth) {
        NearPlane -= d;
    }
}

void Camera::MoveFarPlane(float d)
{
    if (FarPlane - d > NearPlane + minDepth) {
        FarPlane -= d;
    }
}

// Set (and normalize) the camera vectors based on the viewing angles
void Camera::EnforceVectors()
{
    float magnitude;
    Vector temp;

    n.k = sin(Pitch)*cos(Yaw);
    n.i = sin(Pitch)*sin(Yaw);
    n.j = cos(Pitch);

    if(((n.i == 0) && (n.j == 1) && (n.k == 0)) || ((n.i == 0) && (n.j == -1) && (n.k == 0)))
    {
        v.i = 0; v.j = 0; v.k = 1;
    }
    else
    {
        v.i = 0; v.j = 1; v.k = 0;
    }

    temp.i = v.j*n.k - v.k*n.j;
    temp.j = v.k*n.i - v.i*n.k;
    temp.k = v.i*n.j - v.j*n.i;
    magnitude = sqrt(temp.i*temp.i + temp.j*temp.j + temp.k*temp.k);
    u.i = temp.i/magnitude; u.j = temp.j/magnitude; u.k = temp.k/magnitude;

    v.i = n.j*u.k - n.k*u.j;
    v.j = n.k*u.i - n.i*u.k;
    v.k = n.i*u.j - n.j*u.i;
}

// Calculate the new perspective projection matrix; maps into (-1,1)x(-1,1)x(0,1)
void Camera::Perspective()
{
    ProjectionMatrix[0] = ProjectionMatrix[5] = ProjectionMatrix[10] = ProjectionMatrix[15] = 1;
    ProjectionMatrix[1] = ProjectionMatrix[2] = ProjectionMatrix[3] = ProjectionMatrix[4] =
        ProjectionMatrix[6] = ProjectionMatrix[7] = ProjectionMatrix[8] = ProjectionMatrix[9] =
        ProjectionMatrix[11] = ProjectionMatrix[12] = ProjectionMatrix[13] = ProjectionMatrix[14]= 0;

    // Set the non-identity elements
    ProjectionMatrix[0] = 2*ViewPlane/ViewWidth;
    ProjectionMatrix[5] = 2*ViewPlane/ViewHeight;
    ProjectionMatrix[10] = FarPlane/(NearPlane - FarPlane);
    ProjectionMatrix[11] = -1.0;
    ProjectionMatrix[14] = FarPlane*NearPlane/(NearPlane - FarPlane);
    ProjectionMatrix[15] = 0.0;
}

// Calculate the new orthographic projection matrix; maps into (-1,1)x(-1,1)x(0,1)
void Camera::Orthographic()
{
    ProjectionMatrix[0] = ProjectionMatrix[5] = ProjectionMatrix[10] = ProjectionMatrix[15] = 1;
    ProjectionMatrix[1] = ProjectionMatrix[2] = ProjectionMatrix[3] = ProjectionMatrix[4] =
        ProjectionMatrix[6] = ProjectionMatrix[7] = ProjectionMatrix[8] = ProjectionMatrix[9] =
        ProjectionMatrix[11] = ProjectionMatrix[12] = ProjectionMatrix[13] = ProjectionMatrix[14]= 0;

    // Set the non-identity elements
    ProjectionMatrix[0] = 2/ViewWidth;
    ProjectionMatrix[5] = 2/ViewHeight;
    ProjectionMatrix[10] = NearPlane/(NearPlane - FarPlane);
}

void Camera::Reorient() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(-1.1,1.1,-1.1,1.1);

    if (type == PERSPECTIVE) {
        Perspective();
    }
    else if (type == ORTHOGRAPHIC) {
        Orthographic();
    }
    else {
        printf("Camera type must be PERSPECTIVE or ORTHOGRAPHIC.");
        error_exit(1000, "Invalid arguments");
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Calculate the new viewing transform matrix
void Camera::LookAt()
{
    ViewingMatrix[0] = u.i; ViewingMatrix[4] = u.j; ViewingMatrix[8] = u.k;
    ViewingMatrix[12] = -(u.i*Position.x + u.j*Position.y + u.k*Position.z);
    ViewingMatrix[1] = v.i; ViewingMatrix[5] = v.j; ViewingMatrix[9] = v.k;
    ViewingMatrix[13] = -(v.i*Position.x + v.j*Position.y + v.k*Position.z);
    ViewingMatrix[2] = n.i; ViewingMatrix[6] = n.j; ViewingMatrix[10] = n.k;
    ViewingMatrix[14] = -(n.i*Position.x + n.j*Position.y + n.k*Position.z);
    ViewingMatrix[3] = ViewingMatrix[7] = ViewingMatrix[11] = 0; ViewingMatrix[15] = 1;
}

// Clip a polygon against view volume borders
Vertex* Camera::Clip(int count, Vertex* input, int* out_count)
{
    *out_count = 0;
    Vertex* output;
    Vertex* temp;
    if (count > 2) {
        temp = new Vertex[count + 1];
        for(int i = 0; i < count; i++)
        {
            Vertex cur = input[i];
            Vertex next;
            float t = 1000000;
            if (i < count - 1) {
                next = input[i+1];
            }
            else {
                next = input[0];
            }
            Vertex edge = next.Sub(cur);
            bool in = Contains(cur);
            bool nextIn = Contains(next);
            if (in && nextIn) {
                temp[*out_count] = cur;
                *out_count = *out_count + 1;
            }
            else if (in && !nextIn) {
                temp[*out_count] = cur;
                *out_count = *out_count + 1;
                /*if (next.x > 5) {
                t = (5 - cur.x)/edge.x;
                }
                else if (next.x < -5) {
                t = (-5 - cur.x)/edge.x;
                }
                if (next.y > 5) {
                if (abs(t) > abs((5 - cur.y)/edge.y)) {
                t = (5 - cur.y)/edge.y;
                }
                }
                else if (next.y < -5) {
                if (abs(t) > abs((-5 - cur.y)/edge.y)) {
                t = (-5 - cur.y)/edge.y;
                }
                }*/
                if (next.z > FarPlane) {
                    if (abs(t) > abs((FarPlane - cur.z)/edge.z)) {
                        t = (FarPlane - cur.z)/edge.z;
                    }
                }
                else if (next.z < NearPlane) {
                    if (abs(t) > abs((NearPlane - cur.z)/edge.z)) {
                        t = (NearPlane - cur.z)/edge.z;
                    }
                }
                temp[*out_count].x = cur.x + edge.x*t;
                temp[*out_count].y = cur.y + edge.y*t;
                temp[*out_count].z = cur.z + edge.z*t;
                temp[*out_count].h = cur.h + edge.h*t;
                *out_count = *out_count + 1;
            }
            else if (!in && nextIn) {
                /*if (cur.x > 5) {
                t = (5 - cur.x)/edge.x;
                }
                else if (cur.x < -5) {
                t = (-5 - cur.x)/edge.x;
                }
                if (cur.y > 5) {
                if (abs(t) > abs((5 - cur.y)/edge.y)) {
                t = (5 - cur.y)/edge.y;
                }
                }
                else if (cur.y < -5) {
                if (abs(t) > abs((-5 - cur.y)/edge.y)) {
                t = (-5 - cur.y)/edge.y;
                }
                }*/
                if (cur.z > FarPlane) {
                    if (abs(t) > abs((FarPlane - cur.z)/edge.z)) {
                        t = (FarPlane - cur.z)/edge.z;
                    }
                }
                else if (cur.z < NearPlane) {
                    if (abs(t) > abs((NearPlane - cur.z)/edge.z)) {
                        t = (NearPlane - cur.z)/edge.z;
                    }
                }
                temp[*out_count].x = cur.x + edge.x*t;
                temp[*out_count].y = cur.y + edge.y*t;
                temp[*out_count].z = cur.z + edge.z*t;
                temp[*out_count].h = cur.h + edge.h*t;
                *out_count = *out_count + 1;
            }
            else if (!in && !nextIn) {
                // No-op
            }
            else {
                // No-op. Should never happen.
            }
        }
    }
    else if (count == 2) {
        Vertex cur = input[0];
        Vertex next = input[1];
        Vertex edge = next.Sub(cur);
        temp = new Vertex[2];
        float t = 1000000;
        if (Contains(cur) && Contains(next)) {
            temp[*out_count] = cur;
            *out_count = *out_count + 1;
            temp[*out_count] = next;
            *out_count = *out_count + 1;
        }
        else if (Contains(cur) && !Contains(next)) {
            temp[*out_count] = cur;
            *out_count = *out_count + 1;
            /*if (next.x > 5) {
            t = (5 - cur.x)/edge.x;
            }
            else if (next.x < -5) {
            t = (-5 - cur.x)/edge.x;
            }
            if (next.y > 5) {
            if (abs(t) > abs((5 - cur.y)/edge.y)) {
            t = (5 - cur.y)/edge.y;
            }
            }
            else if (next.y < -5) {
            if (abs(t) > abs((-5 - cur.y)/edge.y)) {
            t = (-5 - cur.y)/edge.y;
            }
            }*/
            if (next.z > FarPlane) {
                if (abs(t) > abs((FarPlane - cur.z)/edge.z)) {
                    t = (FarPlane - cur.z)/edge.z;
                }
            }
            else if (next.z < NearPlane) {
                if (abs(t) > abs((NearPlane - cur.z)/edge.z)) {
                    t = (NearPlane - cur.z)/edge.z;
                }
            }
            temp[*out_count].x = cur.x + edge.x*t;
            temp[*out_count].y = cur.y + edge.y*t;
            temp[*out_count].z = cur.z + edge.z*t;
            temp[*out_count].h = cur.h + edge.h*t;
            *out_count = *out_count + 1;
        }
        else if (!Contains(cur) && Contains(next)) {
            /*if (cur.x > 5) {
            t = (5 - cur.x)/edge.x;
            }
            else if (cur.x < -5) {
            t = (-5 - cur.x)/edge.x;
            }
            if (cur.y > 5) {
            if (abs(t) > abs((5 - cur.y)/edge.y)) {
            t = (5 - cur.y)/edge.y;
            }
            }
            else if (cur.y < -5) {
            if (abs(t) > abs((-5 - cur.y)/edge.y)) {
            t = (-5 - cur.y)/edge.y;
            }
            }*/
            if (cur.z > FarPlane) {
                if (abs(t) > abs((FarPlane - cur.z)/edge.z)) {
                    t = (FarPlane - cur.z)/edge.z;
                }
            }
            else if (cur.z < NearPlane) {
                if (abs(t) > abs((NearPlane - cur.z)/edge.z)) {
                    t = (NearPlane - cur.z)/edge.z;
                }
            }
            temp[*out_count].x = cur.x + edge.x*t;
            temp[*out_count].y = cur.y + edge.y*t;
            temp[*out_count].z = cur.z + edge.z*t;
            temp[*out_count].h = cur.h + edge.h*t;
            *out_count = *out_count + 1;
            temp[*out_count] = next;
            *out_count = *out_count + 1;
        }
        else if (!Contains(cur) && !Contains(next)) {
            // No-op
        }
        else {
            // No-op. Should never happen.
        }
    }
    else if (count == 1) {
        Vertex cur = input[0];
        temp = new Vertex[1];
        if (Contains(cur)) {
            temp[*out_count] = cur;
            *out_count = *out_count + 1;
        }
        else {
            // No-op.
        }
    }
    else {
        printf("Argument 'count' must be greater than 0.");
        error_exit(1000, "Invalid arguments");
    }
    output = new Vertex[*out_count];
    for (int i = 0; i < *out_count; i++) {
        output[i] = temp[i];
    }
    return output;
}