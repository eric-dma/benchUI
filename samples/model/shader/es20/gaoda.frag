/*
 * This proprietary software may be used only as
 * authorised by a licensing agreement from Botson Limited
 * (C) COPYRIGHT 2019 Botson Limited
 * ALL RIGHTS RESERVED
 * The entire notice above must be reproduced on all authorised
 * copies and copies may only be made to the extent permitted
 * by a licensing agreement from Botson Limited.
 */

precision lowp float;

varying vec3 vv3colour;

void main()
{
    gl_FragColor = vec4(vv3colour, 1.0);
}
