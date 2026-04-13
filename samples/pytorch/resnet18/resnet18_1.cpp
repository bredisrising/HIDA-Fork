
//===------------------------------------------------------------*- C++ -*-===//
//
// Automatically generated file for High-level Synthesis (HLS).
//
//===----------------------------------------------------------------------===//

#include <algorithm>
#include <ap_axi_sdata.h>
#include <ap_fixed.h>
#include <ap_int.h>
#include <hls_math.h>
#include <hls_stream.h>
#include <hls_vector.h>
#include <math.h>
#include <stdint.h>
#include <string.h>

using namespace std;

void forward_node1(
  float v0[512],
  float v1[8][8],
  float v2[1000],
  float v3[1000],
  float v4[1000],
  int v5,
  int v6
) {	// L66
  #pragma HLS inline
  for (int v7 = 0; v7 < 8; v7 += 1) {	// L67
    for (int v8 = 0; v8 < 8; v8 += 2) {	// L68
      #pragma HLS pipeline II=1
      float v9 = v0[(v7 + (v5 * 8))];	// L69
      float v10 = v1[v7][v8];	// L70
      float v11 = v3[(v8 + (v6 * 8))];	// L71
      float v12 = v9 * v10;	// L72
      float v13 = v11 + v12;	// L73
      v3[(v8 + (v6 * 8))] = v13;	// L74
      float v14 = v2[(v8 + (v6 * 8))];	// L75
      float v15 = v13 + v14;	// L76
      if ((((-v7) + (v5 * -8)) + 511) == 0) {	// L77
        v4[(v8 + (v6 * 8))] = v15;	// L78
      }
      float v16 = v1[v7][(v8 + 1)];	// L80
      float v17 = v3[((v8 + (v6 * 8)) + 1)];	// L81
      float v18 = v9 * v16;	// L82
      float v19 = v17 + v18;	// L83
      v3[((v8 + (v6 * 8)) + 1)] = v19;	// L84
      float v20 = v2[((v8 + (v6 * 8)) + 1)];	// L85
      float v21 = v19 + v20;	// L86
      if ((((-v7) + (v5 * -8)) + 511) == 0) {	// L87
        v4[((v8 + (v6 * 8)) + 1)] = v21;	// L88
      }
    }
  }
}

void forward_node2(
  float v22[512][1000],
  float v23[8][8],
  int v24,
  int v25
) {	// L94
  #pragma HLS inline
  for (int v26 = 0; v26 < 8; v26 += 1) {	// L95
    for (int v27 = 0; v27 < 8; v27 += 2) {	// L96
      #pragma HLS pipeline II=1
      float v28 = v22[(v26 + (v24 * 8))][(v27 + (v25 * 8))];	// L97
      v23[v26][v27] = v28;	// L98
      float v29 = v22[(v26 + (v24 * 8))][((v27 + (v25 * 8)) + 1)];	// L99
      v23[v26][(v27 + 1)] = v29;	// L100
    }
  }
}

void forward_node0(
  float v30[512],
  float v31[512][1000],
  float v32[1000],
  float v33[1000]
) {	// L105
  float v34[1000];	// L106
  #pragma HLS array_partition variable=v34 cyclic factor=2 dim=1
  #pragma HLS bind_storage variable=v34 type=ram_2p impl=lutram

  for (int v35 = 0; v35 < 8000; v35 += 1) {	// L107
    #pragma HLS dataflow
    int v36 = (v35 % 125);	// L108
    int v37 = (v35 / 125);	// L109
    float v38[8][8];	// L110
    #pragma HLS array_partition variable=v38 cyclic factor=2 dim=2
    #pragma HLS bind_storage variable=v38 type=ram_2p impl=lutram

    forward_node2(v31, v38, v37, v36);	// L111
    forward_node1(v30, v38, v32, v34, v33, v37, v36);	// L112
  }
}

void forward_node4(
  float v39[8],
  float v40[512],
  int v41,
  int v42,
  int v43
) {	// L116
  #pragma HLS inline
  for (int v44 = 0; v44 < 8; v44 += 2) {	// L118
    #pragma HLS pipeline II=1
    float v45 = v39[v44];	// L119
    float v46 = v40[(v44 + (v41 * 8))];	// L120
    float v47 = v46 + v45;	// L121
    float v48 = v47 / (float)49.000000;	// L122
    float v49 = (((-v42) + 6) == 0 && ((-v43) + 6) == 0) ? v48 : v47;	// L123
    v40[(v44 + (v41 * 8))] = v49;	// L124
    float v50 = v39[(v44 + 1)];	// L125
    float v51 = v40[((v44 + (v41 * 8)) + 1)];	// L126
    float v52 = v51 + v50;	// L127
    float v53 = v52 / (float)49.000000;	// L128
    float v54 = (((-v42) + 6) == 0 && ((-v43) + 6) == 0) ? v53 : v52;	// L129
    v40[((v44 + (v41 * 8)) + 1)] = v54;	// L130
  }
}

void forward_node5(
  float v55[512][7][7],
  float v56[8],
  int v57,
  int v58,
  int v59
) {	// L134
  #pragma HLS inline
  for (int v60 = 0; v60 < 8; v60 += 2) {	// L135
    #pragma HLS pipeline II=1
    float v61 = v55[(v60 + (v59 * 8))][v57][v58];	// L136
    v56[v60] = v61;	// L137
    float v62 = v55[((v60 + (v59 * 8)) + 1)][v57][v58];	// L138
    v56[(v60 + 1)] = v62;	// L139
  }
}

void forward_node3(
  float v63[512][7][7],
  float v64[512]
) {	// L143
  for (int v65 = 0; v65 < 3136; v65 += 1) {	// L144
    #pragma HLS dataflow
    int v66 = (v65 % 64);	// L145
    int v67 = ((v65 / 64) % 7);	// L146
    int v68 = ((v65 / 64) / 7);	// L147
    float v69[8];	// L148
    #pragma HLS array_partition variable=v69 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v69 type=ram_2p impl=lutram

    forward_node5(v63, v69, v68, v67, v66);	// L149
    forward_node4(v69, v64, v66, v68, v67);	// L150
  }
}

void forward_node7(
  float v70[8],
  float v71[512][7][7],
  int v72,
  int v73,
  int v74
) {	// L154
  #pragma HLS inline
  for (int v75 = 0; v75 < 8; v75 += 1) {	// L155
    #pragma HLS pipeline II=1
    float v76 = v70[v75];	// L156
    v71[(v75 + (v74 * 8))][v72][v73] = v76;	// L157
  }
}

void forward_node8(
  float v77[8],
  float v78[512][7][7],
  int v79,
  int v80,
  int v81
) {	// L161
  #pragma HLS inline
  for (int v82 = 0; v82 < 8; v82 += 1) {	// L162
    #pragma HLS pipeline II=1
    float v83 = v77[v82];	// L163
    v78[(v82 + (v81 * 8))][v79][v80] = v83;	// L164
  }
}

void forward_node9(
  float v84[8],
  float v85[8],
  float v86[8][8],
  float v87[8],
  float v88[8],
  float v89[8],
  int v90,
  int v91,
  int v92
) {	// L168
  #pragma HLS inline
  for (int v93 = 0; v93 < 8; v93 += 1) {	// L170
    for (int v94 = 0; v94 < 8; v94 += 2) {	// L171
      #pragma HLS pipeline II=1
      float v95 = v85[v93];	// L172
      float v96 = v86[v94][v93];	// L173
      float v97 = v87[v94];	// L174
      float v98 = v89[v94];	// L175
      float v99 = (v93 == 0) ? v97 : v98;	// L176
      float v100 = v95 * v96;	// L177
      float v101 = v99 + v100;	// L178
      v89[v94] = v101;	// L179
      float v102 = v84[v94];	// L180
      float v103 = v101 + v102;	// L181
      bool v104 = v103 > (float)0.000000;	// L182
      float v105 = v104 ? v103 : (float)0.000000;	// L183
      if ((((-v93) + (v91 * -8)) + 511) == 0 && ((-v92) + 2) == 0 && ((-v90) + 2) == 0) {	// L184
        v88[v94] = v105;	// L185
      }
      float v106 = v86[(v94 + 1)][v93];	// L187
      float v107 = v87[(v94 + 1)];	// L188
      float v108 = v89[(v94 + 1)];	// L189
      float v109 = (v93 == 0) ? v107 : v108;	// L190
      float v110 = v95 * v106;	// L191
      float v111 = v109 + v110;	// L192
      v89[(v94 + 1)] = v111;	// L193
      float v112 = v84[(v94 + 1)];	// L194
      float v113 = v111 + v112;	// L195
      bool v114 = v113 > (float)0.000000;	// L196
      float v115 = v114 ? v113 : (float)0.000000;	// L197
      if ((((-v93) + (v91 * -8)) + 511) == 0 && ((-v92) + 2) == 0 && ((-v90) + 2) == 0) {	// L198
        v88[(v94 + 1)] = v115;	// L199
      }
    }
  }
}

void forward_node10(
  float v116[512][7][7],
  float v117[8],
  int v118,
  int v119,
  int v120
) {	// L205
  #pragma HLS inline
  for (int v121 = 0; v121 < 8; v121 += 1) {	// L206
    #pragma HLS pipeline II=1
    float v122 = v116[(v121 + (v120 * 8))][v118][v119];	// L207
    v117[v121] = v122;	// L208
  }
}

void forward_node11(
  float v123[512][7][7],
  float v124[8],
  int v125,
  int v126,
  int v127
) {	// L212
  #pragma HLS inline
  for (int v128 = 0; v128 < 8; v128 += 1) {	// L213
    #pragma HLS pipeline II=1
    float v129 = v123[(v128 + (v127 * 8))][v125][v126];	// L214
    v124[v128] = v129;	// L215
  }
}

void forward_node12(
  float v130[512][512][3][3],
  float v131[8][8],
  int v132,
  int v133,
  int v134,
  int v135
) {	// L219
  #pragma HLS inline
  for (int v136 = 0; v136 < 8; v136 += 1) {	// L220
    for (int v137 = 0; v137 < 8; v137 += 2) {	// L221
      #pragma HLS pipeline II=1
      float v138 = v130[(v136 + (v134 * 8))][(v137 + (v135 * 8))][v132][v133];	// L222
      v131[v136][v137] = v138;	// L223
      float v139 = v130[(v136 + (v134 * 8))][((v137 + (v135 * 8)) + 1)][v132][v133];	// L224
      v131[v136][(v137 + 1)] = v139;	// L225
    }
  }
}

void forward_node13(
  float v140[512][7][7],
  float v141[8],
  int v142,
  int v143,
  int v144,
  int v145,
  int v146
) {	// L230
  #pragma HLS inline
  for (int v147 = 0; v147 < 8; v147 += 1) {	// L231
    #pragma HLS pipeline II=1
    float v148 = v140[(v147 + (v142 * 8))][((v143 + v144) - 1)][((v145 + v146) - 1)];	// L232
    v141[v147] = v148;	// L233
  }
}

void forward_node6(
  float v149[512][512][3][3],
  float v150[512][7][7],
  float v151[512][7][7],
  float v152[512][7][7],
  float v153[512][7][7],
  float v154[512][7][7]
) {	// L237
  for (int v155 = 0; v155 < 1806336; v155 += 1) {	// L238
    #pragma HLS dataflow
    int v156 = (v155 % 7);	// L239
    int v157 = ((v155 / 7) % 7);	// L240
    int v158 = (((v155 / 7) / 7) % 64);	// L241
    int v159 = ((((v155 / 7) / 7) / 64) % 3);	// L242
    int v160 = (((((v155 / 7) / 7) / 64) / 3) % 3);	// L243
    int v161 = (((((v155 / 7) / 7) / 64) / 3) / 3);	// L244
    float v162[8];	// L245
    #pragma HLS array_partition variable=v162 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v162 type=ram_2p impl=lutram

    float v163[8];	// L246
    #pragma HLS array_partition variable=v163 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v163 type=ram_2p impl=lutram

    float v164[8];	// L247
    #pragma HLS array_partition variable=v164 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v164 type=ram_2p impl=lutram

    float v165[8][8];	// L248
    #pragma HLS array_partition variable=v165 cyclic factor=2 dim=1
    #pragma HLS array_partition variable=v165 cyclic factor=2 dim=2
    #pragma HLS bind_storage variable=v165 type=ram_2p impl=lutram

    float v166[8];	// L249
    #pragma HLS bind_storage variable=v166 type=ram_t2p impl=bram

    forward_node13(v150, v166, v161, v157, v160, v156, v159);	// L250
    forward_node12(v149, v165, v160, v159, v158, v161);	// L251
    forward_node11(v152, v164, v157, v156, v158);	// L252
    forward_node10(v151, v163, v157, v156, v158);	// L253
    float v167[8];	// L254
    #pragma HLS array_partition variable=v167 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v167 type=ram_2p impl=lutram

    forward_node9(v163, v166, v165, v164, v162, v167, v159, v161, v160);	// L255
    forward_node8(v167, v154, v157, v156, v158);	// L256
    forward_node7(v162, v153, v157, v156, v158);	// L257
  }
}

void forward_node15(
  float v168[8],
  float v169[512][7][7],
  int v170,
  int v171,
  int v172
) {	// L261
  #pragma HLS inline
  for (int v173 = 0; v173 < 8; v173 += 1) {	// L262
    #pragma HLS pipeline II=1
    float v174 = v168[v173];	// L263
    v169[(v173 + (v172 * 8))][v170][v171] = v174;	// L264
  }
}

void forward_node16(
  float v175[8],
  float v176[8][8],
  float v177[8],
  float v178[8],
  float v179[8],
  int v180,
  int v181,
  int v182
) {	// L268
  #pragma HLS inline
  for (int v183 = 0; v183 < 8; v183 += 1) {	// L270
    for (int v184 = 0; v184 < 8; v184 += 2) {	// L271
      #pragma HLS pipeline II=1
      float v185 = v177[v184];	// L272
      float v186 = v178[v184];	// L273
      float v187 = v179[v184];	// L274
      float v188 = (v183 == 0) ? v186 : v187;	// L275
      float v189 = ((v183 + (v180 * 8)) == 0 && v182 == 0 && v181 == 0) ? v185 : v188;	// L276
      float v190 = v175[v183];	// L277
      float v191 = v176[v184][v183];	// L278
      float v192 = v190 * v191;	// L279
      float v193 = v189 + v192;	// L280
      bool v194 = v193 > (float)0.000000;	// L281
      float v195 = v194 ? v193 : (float)0.000000;	// L282
      float v196 = ((((-v183) + (v180 * -8)) + 511) == 0 && ((-v182) + 2) == 0 && ((-v181) + 2) == 0) ? v195 : v193;	// L283
      v179[v184] = v196;	// L284
      float v197 = v177[(v184 + 1)];	// L285
      float v198 = v178[(v184 + 1)];	// L286
      float v199 = v179[(v184 + 1)];	// L287
      float v200 = (v183 == 0) ? v198 : v199;	// L288
      float v201 = ((v183 + (v180 * 8)) == 0 && v182 == 0 && v181 == 0) ? v197 : v200;	// L289
      float v202 = v176[(v184 + 1)][v183];	// L290
      float v203 = v190 * v202;	// L291
      float v204 = v201 + v203;	// L292
      bool v205 = v204 > (float)0.000000;	// L293
      float v206 = v205 ? v204 : (float)0.000000;	// L294
      float v207 = ((((-v183) + (v180 * -8)) + 511) == 0 && ((-v182) + 2) == 0 && ((-v181) + 2) == 0) ? v206 : v204;	// L295
      v179[(v184 + 1)] = v207;	// L296
    }
  }
}

void forward_node17(
  float v208[512][512][3][3],
  float v209[8][8],
  int v210,
  int v211,
  int v212,
  int v213
) {	// L301
  #pragma HLS inline
  for (int v214 = 0; v214 < 8; v214 += 1) {	// L302
    for (int v215 = 0; v215 < 8; v215 += 2) {	// L303
      #pragma HLS pipeline II=1
      float v216 = v208[(v214 + (v212 * 8))][(v215 + (v213 * 8))][v210][v211];	// L304
      v209[v214][v215] = v216;	// L305
      float v217 = v208[(v214 + (v212 * 8))][((v215 + (v213 * 8)) + 1)][v210][v211];	// L306
      v209[v214][(v215 + 1)] = v217;	// L307
    }
  }
}

void forward_node18(
  float v218[512][7][7],
  float v219[8],
  int v220,
  int v221,
  int v222,
  int v223,
  int v224
) {	// L312
  #pragma HLS inline
  for (int v225 = 0; v225 < 8; v225 += 1) {	// L313
    #pragma HLS pipeline II=1
    float v226 = v218[(v225 + (v220 * 8))][((v221 + v222) - 1)][((v223 + v224) - 1)];	// L314
    v219[v225] = v226;	// L315
  }
}

void forward_node19(
  float v227[512][7][7],
  float v228[8],
  int v229,
  int v230,
  int v231
) {	// L319
  #pragma HLS inline
  for (int v232 = 0; v232 < 8; v232 += 1) {	// L320
    #pragma HLS pipeline II=1
    float v233 = v227[(v232 + (v231 * 8))][v229][v230];	// L321
    v228[v232] = v233;	// L322
  }
}

void forward_node20(
  float v234[512][7][7],
  float v235[8],
  int v236,
  int v237,
  int v238
) {	// L326
  #pragma HLS inline
  for (int v239 = 0; v239 < 8; v239 += 1) {	// L327
    #pragma HLS pipeline II=1
    float v240 = v234[(v239 + (v238 * 8))][v236][v237];	// L328
    v235[v239] = v240;	// L329
  }
}

void forward_node14(
  float v241[512][7][7],
  float v242[512][512][3][3],
  float v243[512][7][7],
  float v244[512][7][7],
  float v245[512][7][7]
) {	// L333
  for (int v246 = 0; v246 < 1806336; v246 += 1) {	// L334
    #pragma HLS dataflow
    int v247 = (v246 % 7);	// L335
    int v248 = ((v246 / 7) % 7);	// L336
    int v249 = (((v246 / 7) / 7) % 64);	// L337
    int v250 = ((((v246 / 7) / 7) / 64) % 3);	// L338
    int v251 = (((((v246 / 7) / 7) / 64) / 3) % 3);	// L339
    int v252 = (((((v246 / 7) / 7) / 64) / 3) / 3);	// L340
    float v253[8][8];	// L341
    #pragma HLS array_partition variable=v253 cyclic factor=2 dim=1
    #pragma HLS array_partition variable=v253 cyclic factor=2 dim=2
    #pragma HLS bind_storage variable=v253 type=ram_2p impl=lutram

    float v254[8];	// L342
    #pragma HLS bind_storage variable=v254 type=ram_t2p impl=bram

    float v255[8];	// L343
    #pragma HLS array_partition variable=v255 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v255 type=ram_2p impl=lutram

    float v256[8];	// L344
    #pragma HLS array_partition variable=v256 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v256 type=ram_2p impl=lutram

    forward_node20(v241, v256, v248, v247, v249);	// L345
    forward_node19(v244, v255, v248, v247, v249);	// L346
    forward_node18(v243, v254, v252, v248, v251, v247, v250);	// L347
    forward_node17(v242, v253, v251, v250, v249, v252);	// L348
    float v257[8];	// L349
    #pragma HLS array_partition variable=v257 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v257 type=ram_2p impl=lutram

    forward_node16(v254, v253, v256, v255, v257, v252, v250, v251);	// L350
    forward_node15(v257, v245, v248, v247, v249);	// L351
  }
}

void forward_node22(
  float v258[8],
  float v259[512][7][7],
  int v260,
  int v261,
  int v262
) {	// L355
  #pragma HLS inline
  for (int v263 = 0; v263 < 8; v263 += 1) {	// L356
    #pragma HLS pipeline II=1
    float v264 = v258[v263];	// L357
    v259[(v263 + (v262 * 8))][v260][v261] = v264;	// L358
  }
}

void forward_node23(
  float v265[8],
  float v266[512][7][7],
  int v267,
  int v268,
  int v269
) {	// L362
  #pragma HLS inline
  for (int v270 = 0; v270 < 8; v270 += 1) {	// L363
    #pragma HLS pipeline II=1
    float v271 = v265[v270];	// L364
    v266[(v270 + (v269 * 8))][v267][v268] = v271;	// L365
  }
}

void forward_node24(
  float v272[8],
  float v273[8][8],
  float v274[8],
  float v275[8],
  float v276[8],
  float v277[8],
  float v278[8],
  int v279
) {	// L369
  #pragma HLS inline
  for (int v280 = 0; v280 < 8; v280 += 1) {	// L371
    for (int v281 = 0; v281 < 8; v281 += 2) {	// L372
      #pragma HLS pipeline II=1
      float v282 = v272[v281];	// L373
      float v283 = v276[v281];	// L374
      float v284 = v277[v281];	// L375
      float v285 = (v280 == 0) ? v283 : v284;	// L376
      float v286 = ((v280 + (v279 * 8)) == 0) ? v282 : v285;	// L377
      float v287 = v275[v280];	// L378
      float v288 = v273[v281][v280];	// L379
      float v289 = v287 * v288;	// L380
      float v290 = v286 + v289;	// L381
      v277[v281] = v290;	// L382
      float v291 = v274[v281];	// L383
      float v292 = v291 + v290;	// L384
      bool v293 = v292 > (float)0.000000;	// L385
      float v294 = v293 ? v292 : (float)0.000000;	// L386
      if ((((-v280) + (v279 * -8)) + 255) == 0) {	// L387
        v278[v281] = v294;	// L388
      }
      float v295 = v272[(v281 + 1)];	// L390
      float v296 = v276[(v281 + 1)];	// L391
      float v297 = v277[(v281 + 1)];	// L392
      float v298 = (v280 == 0) ? v296 : v297;	// L393
      float v299 = ((v280 + (v279 * 8)) == 0) ? v295 : v298;	// L394
      float v300 = v273[(v281 + 1)][v280];	// L395
      float v301 = v287 * v300;	// L396
      float v302 = v299 + v301;	// L397
      v277[(v281 + 1)] = v302;	// L398
      float v303 = v274[(v281 + 1)];	// L399
      float v304 = v303 + v302;	// L400
      bool v305 = v304 > (float)0.000000;	// L401
      float v306 = v305 ? v304 : (float)0.000000;	// L402
      if ((((-v280) + (v279 * -8)) + 255) == 0) {	// L403
        v278[(v281 + 1)] = v306;	// L404
      }
    }
  }
}

void forward_node25(
  float v307[512][7][7],
  float v308[8],
  int v309,
  int v310,
  int v311
) {	// L410
  #pragma HLS inline
  for (int v312 = 0; v312 < 8; v312 += 1) {	// L411
    #pragma HLS pipeline II=1
    float v313 = v307[(v312 + (v311 * 8))][v309][v310];	// L412
    v308[v312] = v313;	// L413
  }
}

void forward_node26(
  float v314[512][256],
  float v315[8][8],
  int v316,
  int v317
) {	// L417
  #pragma HLS inline
  for (int v318 = 0; v318 < 8; v318 += 1) {	// L418
    for (int v319 = 0; v319 < 8; v319 += 2) {	// L419
      #pragma HLS pipeline II=1
      float v320 = v314[(v318 + (v316 * 8))][(v319 + (v317 * 8))];	// L420
      v315[v318][v319] = v320;	// L421
      float v321 = v314[(v318 + (v316 * 8))][((v319 + (v317 * 8)) + 1)];	// L422
      v315[v318][(v319 + 1)] = v321;	// L423
    }
  }
}

void forward_node27(
  float v322[256][14][14],
  float v323[8],
  int v324,
  int v325,
  int v326
) {	// L428
  #pragma HLS inline
  for (int v327 = 0; v327 < 8; v327 += 1) {	// L429
    #pragma HLS pipeline II=1
    float v328 = v322[(v327 + (v324 * 8))][(v325 * 2)][(v326 * 2)];	// L430
    v323[v327] = v328;	// L431
  }
}

void forward_node28(
  float v329[512][7][7],
  float v330[8],
  int v331,
  int v332,
  int v333
) {	// L435
  #pragma HLS inline
  for (int v334 = 0; v334 < 8; v334 += 1) {	// L436
    #pragma HLS pipeline II=1
    float v335 = v329[(v334 + (v333 * 8))][v331][v332];	// L437
    v330[v334] = v335;	// L438
  }
}

void forward_node29(
  float v336[512][7][7],
  float v337[8],
  int v338,
  int v339,
  int v340
) {	// L442
  #pragma HLS inline
  for (int v341 = 0; v341 < 8; v341 += 1) {	// L443
    #pragma HLS pipeline II=1
    float v342 = v336[(v341 + (v340 * 8))][v338][v339];	// L444
    v337[v341] = v342;	// L445
  }
}

void forward_node21(
  float v343[256][14][14],
  float v344[512][7][7],
  float v345[512][7][7],
  float v346[512][256],
  float v347[512][7][7],
  float v348[512][7][7],
  float v349[512][7][7]
) {	// L449
  for (int v350 = 0; v350 < 100352; v350 += 1) {	// L450
    #pragma HLS dataflow
    int v351 = (v350 % 7);	// L451
    int v352 = ((v350 / 7) % 7);	// L452
    int v353 = (((v350 / 7) / 7) % 64);	// L453
    int v354 = (((v350 / 7) / 7) / 64);	// L454
    float v355[8];	// L455
    #pragma HLS array_partition variable=v355 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v355 type=ram_2p impl=lutram

    float v356[8];	// L456
    #pragma HLS array_partition variable=v356 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v356 type=ram_2p impl=lutram

    float v357[8][8];	// L457
    #pragma HLS array_partition variable=v357 cyclic factor=2 dim=1
    #pragma HLS array_partition variable=v357 cyclic factor=2 dim=2
    #pragma HLS bind_storage variable=v357 type=ram_2p impl=lutram

    float v358[8];	// L458
    #pragma HLS bind_storage variable=v358 type=ram_t2p impl=bram

    float v359[8];	// L459
    #pragma HLS array_partition variable=v359 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v359 type=ram_2p impl=lutram

    float v360[8];	// L460
    #pragma HLS array_partition variable=v360 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v360 type=ram_2p impl=lutram

    forward_node29(v344, v360, v352, v351, v353);	// L461
    forward_node28(v347, v359, v352, v351, v353);	// L462
    forward_node27(v343, v358, v354, v352, v351);	// L463
    forward_node26(v346, v357, v353, v354);	// L464
    forward_node25(v345, v356, v352, v351, v353);	// L465
    float v361[8];	// L466
    #pragma HLS array_partition variable=v361 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v361 type=ram_2p impl=lutram

    forward_node24(v360, v357, v356, v358, v359, v361, v355, v354);	// L467
    forward_node23(v361, v348, v352, v351, v353);	// L468
    forward_node22(v355, v349, v352, v351, v353);	// L469
  }
}

void forward_node31(
  float v362[8],
  float v363[512][7][7],
  int v364,
  int v365,
  int v366
) {	// L473
  #pragma HLS inline
  for (int v367 = 0; v367 < 8; v367 += 1) {	// L474
    #pragma HLS pipeline II=1
    float v368 = v362[v367];	// L475
    v363[(v367 + (v366 * 8))][v364][v365] = v368;	// L476
  }
}

void forward_node32(
  float v369[8],
  float v370[8],
  float v371[8][8],
  float v372[8],
  float v373[8],
  int v374,
  int v375,
  int v376
) {	// L480
  #pragma HLS inline
  for (int v377 = 0; v377 < 8; v377 += 1) {	// L481
    for (int v378 = 0; v378 < 8; v378 += 2) {	// L482
      #pragma HLS pipeline II=1
      float v379 = v369[v378];	// L483
      float v380 = v372[v378];	// L484
      float v381 = v373[v378];	// L485
      float v382 = (v377 == 0) ? v380 : v381;	// L486
      float v383 = ((v377 + (v376 * 8)) == 0 && v374 == 0 && v375 == 0) ? v379 : v382;	// L487
      float v384 = v370[v377];	// L488
      float v385 = v371[v378][v377];	// L489
      float v386 = v384 * v385;	// L490
      float v387 = v383 + v386;	// L491
      v373[v378] = v387;	// L492
      float v388 = v369[(v378 + 1)];	// L493
      float v389 = v372[(v378 + 1)];	// L494
      float v390 = v373[(v378 + 1)];	// L495
      float v391 = (v377 == 0) ? v389 : v390;	// L496
      float v392 = ((v377 + (v376 * 8)) == 0 && v374 == 0 && v375 == 0) ? v388 : v391;	// L497
      float v393 = v371[(v378 + 1)][v377];	// L498
      float v394 = v384 * v393;	// L499
      float v395 = v392 + v394;	// L500
      v373[(v378 + 1)] = v395;	// L501
    }
  }
}

void forward_node33(
  float v396[512][512][3][3],
  float v397[8][8],
  int v398,
  int v399,
  int v400,
  int v401
) {	// L506
  #pragma HLS inline
  for (int v402 = 0; v402 < 8; v402 += 1) {	// L507
    for (int v403 = 0; v403 < 8; v403 += 2) {	// L508
      #pragma HLS pipeline II=1
      float v404 = v396[(v402 + (v400 * 8))][(v403 + (v401 * 8))][v398][v399];	// L509
      v397[v402][v403] = v404;	// L510
      float v405 = v396[(v402 + (v400 * 8))][((v403 + (v401 * 8)) + 1)][v398][v399];	// L511
      v397[v402][(v403 + 1)] = v405;	// L512
    }
  }
}

void forward_node34(
  float v406[512][7][7],
  float v407[8],
  int v408,
  int v409,
  int v410,
  int v411,
  int v412
) {	// L517
  #pragma HLS inline
  for (int v413 = 0; v413 < 8; v413 += 1) {	// L518
    #pragma HLS pipeline II=1
    float v414 = v406[(v413 + (v408 * 8))][((v409 + v410) - 1)][((v411 + v412) - 1)];	// L519
    v407[v413] = v414;	// L520
  }
}

void forward_node35(
  float v415[512][7][7],
  float v416[8],
  int v417,
  int v418,
  int v419
) {	// L524
  #pragma HLS inline
  for (int v420 = 0; v420 < 8; v420 += 1) {	// L525
    #pragma HLS pipeline II=1
    float v421 = v415[(v420 + (v419 * 8))][v417][v418];	// L526
    v416[v420] = v421;	// L527
  }
}

void forward_node36(
  float v422[512][7][7],
  float v423[8],
  int v424,
  int v425,
  int v426
) {	// L531
  #pragma HLS inline
  for (int v427 = 0; v427 < 8; v427 += 1) {	// L532
    #pragma HLS pipeline II=1
    float v428 = v422[(v427 + (v426 * 8))][v424][v425];	// L533
    v423[v427] = v428;	// L534
  }
}

void forward_node30(
  float v429[512][512][3][3],
  float v430[512][7][7],
  float v431[512][7][7],
  float v432[512][7][7],
  float v433[512][7][7]
) {	// L538
  for (int v434 = 0; v434 < 1806336; v434 += 1) {	// L539
    #pragma HLS dataflow
    int v435 = (v434 % 7);	// L540
    int v436 = ((v434 / 7) % 7);	// L541
    int v437 = (((v434 / 7) / 7) % 64);	// L542
    int v438 = ((((v434 / 7) / 7) / 64) % 3);	// L543
    int v439 = (((((v434 / 7) / 7) / 64) / 3) % 3);	// L544
    int v440 = (((((v434 / 7) / 7) / 64) / 3) / 3);	// L545
    float v441[8][8];	// L546
    #pragma HLS array_partition variable=v441 cyclic factor=2 dim=1
    #pragma HLS array_partition variable=v441 cyclic factor=2 dim=2
    #pragma HLS bind_storage variable=v441 type=ram_2p impl=lutram

    float v442[8];	// L547
    #pragma HLS bind_storage variable=v442 type=ram_t2p impl=bram

    float v443[8];	// L548
    #pragma HLS array_partition variable=v443 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v443 type=ram_2p impl=lutram

    float v444[8];	// L549
    #pragma HLS array_partition variable=v444 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v444 type=ram_2p impl=lutram

    forward_node36(v430, v444, v436, v435, v437);	// L550
    forward_node35(v432, v443, v436, v435, v437);	// L551
    forward_node34(v431, v442, v440, v436, v439, v435, v438);	// L552
    forward_node33(v429, v441, v439, v438, v437, v440);	// L553
    float v445[8];	// L554
    #pragma HLS array_partition variable=v445 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v445 type=ram_2p impl=lutram

    forward_node32(v444, v442, v441, v443, v445, v439, v438, v440);	// L555
    forward_node31(v445, v433, v436, v435, v437);	// L556
  }
}

void forward_node38(
  float v446[8],
  float v447[512][7][7],
  int v448,
  int v449,
  int v450
) {	// L560
  #pragma HLS inline
  for (int v451 = 0; v451 < 8; v451 += 1) {	// L561
    #pragma HLS pipeline II=1
    float v452 = v446[v451];	// L562
    v447[(v451 + (v450 * 8))][v448][v449] = v452;	// L563
  }
}

void forward_node39(
  float v453[8][8],
  float v454[8],
  float v455[8],
  float v456[8],
  float v457[8],
  int v458,
  int v459,
  int v460
) {	// L567
  #pragma HLS inline
  for (int v461 = 0; v461 < 8; v461 += 1) {	// L569
    for (int v462 = 0; v462 < 8; v462 += 2) {	// L570
      #pragma HLS pipeline II=1
      float v463 = v455[v462];	// L571
      float v464 = v456[v462];	// L572
      float v465 = v457[v462];	// L573
      float v466 = (v461 == 0) ? v464 : v465;	// L574
      float v467 = ((v461 + (v460 * 8)) == 0 && v459 == 0 && v458 == 0) ? v463 : v466;	// L575
      float v468 = v454[v461];	// L576
      float v469 = v453[v462][v461];	// L577
      float v470 = v468 * v469;	// L578
      float v471 = v467 + v470;	// L579
      bool v472 = v471 > (float)0.000000;	// L580
      float v473 = v472 ? v471 : (float)0.000000;	// L581
      float v474 = ((((-v461) + (v460 * -8)) + 255) == 0 && ((-v459) + 2) == 0 && ((-v458) + 2) == 0) ? v473 : v471;	// L582
      v457[v462] = v474;	// L583
      float v475 = v455[(v462 + 1)];	// L584
      float v476 = v456[(v462 + 1)];	// L585
      float v477 = v457[(v462 + 1)];	// L586
      float v478 = (v461 == 0) ? v476 : v477;	// L587
      float v479 = ((v461 + (v460 * 8)) == 0 && v459 == 0 && v458 == 0) ? v475 : v478;	// L588
      float v480 = v453[(v462 + 1)][v461];	// L589
      float v481 = v468 * v480;	// L590
      float v482 = v479 + v481;	// L591
      bool v483 = v482 > (float)0.000000;	// L592
      float v484 = v483 ? v482 : (float)0.000000;	// L593
      float v485 = ((((-v461) + (v460 * -8)) + 255) == 0 && ((-v459) + 2) == 0 && ((-v458) + 2) == 0) ? v484 : v482;	// L594
      v457[(v462 + 1)] = v485;	// L595
    }
  }
}

void forward_node40(
  float v486[512][256][3][3],
  float v487[8][8],
  int v488,
  int v489,
  int v490,
  int v491
) {	// L600
  #pragma HLS inline
  for (int v492 = 0; v492 < 8; v492 += 1) {	// L601
    for (int v493 = 0; v493 < 8; v493 += 2) {	// L602
      #pragma HLS pipeline II=1
      float v494 = v486[(v492 + (v490 * 8))][(v493 + (v491 * 8))][v488][v489];	// L603
      v487[v492][v493] = v494;	// L604
      float v495 = v486[(v492 + (v490 * 8))][((v493 + (v491 * 8)) + 1)][v488][v489];	// L605
      v487[v492][(v493 + 1)] = v495;	// L606
    }
  }
}

void forward_node41(
  float v496[256][14][14],
  float v497[8],
  int v498,
  int v499,
  int v500,
  int v501,
  int v502
) {	// L611
  #pragma HLS inline
  for (int v503 = 0; v503 < 8; v503 += 1) {	// L612
    #pragma HLS pipeline II=1
    float v504 = v496[(v503 + (v498 * 8))][(((v499 * 2) + v500) - 1)][(((v501 * 2) + v502) - 1)];	// L613
    v497[v503] = v504;	// L614
  }
}

void forward_node42(
  float v505[512][7][7],
  float v506[8],
  int v507,
  int v508,
  int v509
) {	// L618
  #pragma HLS inline
  for (int v510 = 0; v510 < 8; v510 += 1) {	// L619
    #pragma HLS pipeline II=1
    float v511 = v505[(v510 + (v509 * 8))][v507][v508];	// L620
    v506[v510] = v511;	// L621
  }
}

void forward_node43(
  float v512[512][7][7],
  float v513[8],
  int v514,
  int v515,
  int v516
) {	// L625
  #pragma HLS inline
  for (int v517 = 0; v517 < 8; v517 += 1) {	// L626
    #pragma HLS pipeline II=1
    float v518 = v512[(v517 + (v516 * 8))][v514][v515];	// L627
    v513[v517] = v518;	// L628
  }
}

void forward_node37(
  float v519[256][14][14],
  float v520[512][7][7],
  float v521[512][256][3][3],
  float v522[512][7][7],
  float v523[512][7][7]
) {	// L632
  for (int v524 = 0; v524 < 903168; v524 += 1) {	// L633
    #pragma HLS dataflow
    int v525 = (v524 % 7);	// L634
    int v526 = ((v524 / 7) % 7);	// L635
    int v527 = (((v524 / 7) / 7) % 64);	// L636
    int v528 = ((((v524 / 7) / 7) / 64) % 3);	// L637
    int v529 = (((((v524 / 7) / 7) / 64) / 3) % 3);	// L638
    int v530 = (((((v524 / 7) / 7) / 64) / 3) / 3);	// L639
    float v531[8][8];	// L640
    #pragma HLS array_partition variable=v531 cyclic factor=2 dim=1
    #pragma HLS array_partition variable=v531 cyclic factor=2 dim=2
    #pragma HLS bind_storage variable=v531 type=ram_2p impl=lutram

    float v532[8];	// L641
    #pragma HLS bind_storage variable=v532 type=ram_t2p impl=bram

    float v533[8];	// L642
    #pragma HLS array_partition variable=v533 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v533 type=ram_2p impl=lutram

    float v534[8];	// L643
    #pragma HLS array_partition variable=v534 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v534 type=ram_2p impl=lutram

    forward_node43(v520, v534, v526, v525, v527);	// L644
    forward_node42(v522, v533, v526, v525, v527);	// L645
    forward_node41(v519, v532, v530, v526, v529, v525, v528);	// L646
    forward_node40(v521, v531, v529, v528, v527, v530);	// L647
    float v535[8];	// L648
    #pragma HLS array_partition variable=v535 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v535 type=ram_2p impl=lutram

    forward_node39(v531, v532, v534, v533, v535, v528, v529, v530);	// L649
    forward_node38(v535, v523, v526, v525, v527);	// L650
  }
}

void forward_node45(
  float v536[8][7][7],
  float v537[256][14][14],
  int v538,
  int v539,
  int v540
) {	// L654
  #pragma HLS inline
  for (int v541 = 0; v541 < 8; v541 += 1) {	// L655
    for (int v542 = 0; v542 < 7; v542 += 1) {	// L656
      for (int v543 = 0; v543 < 7; v543 += 1) {	// L657
        #pragma HLS pipeline II=1
        float v544 = v536[v541][v542][v543];	// L658
        v537[(v541 + (v538 * 8))][(v542 + (v539 * 7))][(v543 + (v540 * 7))] = v544;	// L659
      }
    }
  }
}

void forward_node46(
  float v545[8][7][7],
  float v546[256][14][14],
  int v547,
  int v548,
  int v549
) {	// L665
  #pragma HLS inline
  for (int v550 = 0; v550 < 8; v550 += 1) {	// L666
    for (int v551 = 0; v551 < 7; v551 += 1) {	// L667
      for (int v552 = 0; v552 < 7; v552 += 1) {	// L668
        #pragma HLS pipeline II=1
        float v553 = v545[v550][v551][v552];	// L669
        v546[(v550 + (v547 * 8))][(v551 + (v548 * 7))][(v552 + (v549 * 7))] = v553;	// L670
      }
    }
  }
}

void forward_node47(
  float v554[8][7][7],
  float v555[8][7][7],
  float v556[8][8],
  float v557[8][7][7],
  float v558[8][7][7],
  float v559[8][7][7],
  int v560,
  int v561,
  int v562
) {	// L676
  #pragma HLS inline
  for (int v563 = 0; v563 < 8; v563 += 1) {	// L678
    for (int v564 = 0; v564 < 8; v564 += 2) {	// L679
      for (int v565 = 0; v565 < 7; v565 += 1) {	// L680
        for (int v566 = 0; v566 < 7; v566 += 1) {	// L681
          #pragma HLS pipeline II=1
          float v567 = v555[v563][v565][v566];	// L682
          float v568 = v556[v564][v563];	// L683
          float v569 = v557[v564][v565][v566];	// L684
          float v570 = v559[v564][v565][v566];	// L685
          float v571 = (v563 == 0) ? v569 : v570;	// L686
          float v572 = v567 * v568;	// L687
          float v573 = v571 + v572;	// L688
          v559[v564][v565][v566] = v573;	// L689
          float v574 = v554[v564][v565][v566];	// L690
          float v575 = v573 + v574;	// L691
          bool v576 = v575 > (float)0.000000;	// L692
          float v577 = v576 ? v575 : (float)0.000000;	// L693
          if ((((-v563) + (v561 * -8)) + 255) == 0 && ((-v562) + 2) == 0 && ((-v560) + 2) == 0) {	// L694
            v558[v564][v565][v566] = v577;	// L695
          }
          float v578 = v556[(v564 + 1)][v563];	// L697
          float v579 = v557[(v564 + 1)][v565][v566];	// L698
          float v580 = v559[(v564 + 1)][v565][v566];	// L699
          float v581 = (v563 == 0) ? v579 : v580;	// L700
          float v582 = v567 * v578;	// L701
          float v583 = v581 + v582;	// L702
          v559[(v564 + 1)][v565][v566] = v583;	// L703
          float v584 = v554[(v564 + 1)][v565][v566];	// L704
          float v585 = v583 + v584;	// L705
          bool v586 = v585 > (float)0.000000;	// L706
          float v587 = v586 ? v585 : (float)0.000000;	// L707
          if ((((-v563) + (v561 * -8)) + 255) == 0 && ((-v562) + 2) == 0 && ((-v560) + 2) == 0) {	// L708
            v558[(v564 + 1)][v565][v566] = v587;	// L709
          }
        }
      }
    }
  }
}

void forward_node48(
  float v588[256][14][14],
  float v589[8][7][7],
  int v590,
  int v591,
  int v592
) {	// L717
  #pragma HLS inline
  for (int v593 = 0; v593 < 8; v593 += 1) {	// L718
    for (int v594 = 0; v594 < 7; v594 += 1) {	// L719
      for (int v595 = 0; v595 < 7; v595 += 1) {	// L720
        #pragma HLS pipeline II=1
        float v596 = v588[(v593 + (v590 * 8))][(v594 + (v591 * 7))][(v595 + (v592 * 7))];	// L721
        v589[v593][v594][v595] = v596;	// L722
      }
    }
  }
}

void forward_node49(
  float v597[256][14][14],
  float v598[8][7][7],
  int v599,
  int v600,
  int v601
) {	// L728
  #pragma HLS inline
  for (int v602 = 0; v602 < 8; v602 += 1) {	// L729
    for (int v603 = 0; v603 < 7; v603 += 1) {	// L730
      for (int v604 = 0; v604 < 7; v604 += 1) {	// L731
        #pragma HLS pipeline II=1
        float v605 = v597[(v602 + (v599 * 8))][(v603 + (v600 * 7))][(v604 + (v601 * 7))];	// L732
        v598[v602][v603][v604] = v605;	// L733
      }
    }
  }
}

void forward_node50(
  float v606[256][256][3][3],
  float v607[8][8],
  int v608,
  int v609,
  int v610,
  int v611
) {	// L739
  #pragma HLS inline
  for (int v612 = 0; v612 < 8; v612 += 1) {	// L740
    for (int v613 = 0; v613 < 8; v613 += 1) {	// L741
      #pragma HLS pipeline II=1
      float v614 = v606[(v612 + (v610 * 8))][(v613 + (v611 * 8))][v608][v609];	// L742
      v607[v612][v613] = v614;	// L743
    }
  }
}

void forward_node51(
  float v615[256][14][14],
  float v616[8][7][7],
  int v617,
  int v618,
  int v619,
  int v620,
  int v621
) {	// L748
  #pragma HLS inline
  for (int v622 = 0; v622 < 8; v622 += 1) {	// L749
    for (int v623 = 0; v623 < 7; v623 += 1) {	// L750
      for (int v624 = 0; v624 < 7; v624 += 1) {	// L751
        #pragma HLS pipeline II=1
        float v625 = v615[(v622 + (v617 * 8))][(((v623 + v618) + (v619 * 7)) - 1)][(((v624 + v620) + (v621 * 7)) - 1)];	// L752
        v616[v622][v623][v624] = v625;	// L753
      }
    }
  }
}

void forward_node44(
  float v626[256][14][14],
  float v627[256][256][3][3],
  float v628[256][14][14],
  float v629[256][14][14],
  float v630[256][14][14],
  float v631[256][14][14]
) {	// L759
  for (int v632 = 0; v632 < 36864; v632 += 1) {	// L760
    #pragma HLS dataflow
    int v633 = (v632 % 2);	// L761
    int v634 = ((v632 / 2) % 2);	// L762
    int v635 = (((v632 / 2) / 2) % 32);	// L763
    int v636 = ((((v632 / 2) / 2) / 32) % 3);	// L764
    int v637 = (((((v632 / 2) / 2) / 32) / 3) % 3);	// L765
    int v638 = (((((v632 / 2) / 2) / 32) / 3) / 3);	// L766
    float v639[8][7][7];	// L767
    #pragma HLS array_partition variable=v639 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v639 type=ram_2p impl=lutram

    float v640[8][7][7];	// L768
    #pragma HLS array_partition variable=v640 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v640 type=ram_2p impl=lutram

    float v641[8][7][7];	// L769
    #pragma HLS array_partition variable=v641 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v641 type=ram_2p impl=lutram

    float v642[8][8];	// L770
    #pragma HLS array_partition variable=v642 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v642 type=ram_2p impl=lutram

    float v643[8][7][7];	// L771
    #pragma HLS bind_storage variable=v643 type=ram_t2p impl=bram

    forward_node51(v626, v643, v638, v637, v634, v636, v633);	// L772
    forward_node50(v627, v642, v637, v636, v635, v638);	// L773
    forward_node49(v629, v641, v635, v634, v633);	// L774
    forward_node48(v628, v640, v635, v634, v633);	// L775
    float v644[8][7][7];	// L776
    #pragma HLS array_partition variable=v644 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v644 type=ram_2p impl=lutram

    forward_node47(v640, v643, v642, v641, v639, v644, v636, v638, v637);	// L777
    forward_node46(v644, v631, v635, v634, v633);	// L778
    forward_node45(v639, v630, v635, v634, v633);	// L779
  }
}

void forward_node53(
  float v645[8][7][7],
  float v646[256][14][14],
  int v647,
  int v648,
  int v649
) {	// L783
  #pragma HLS inline
  for (int v650 = 0; v650 < 8; v650 += 1) {	// L784
    for (int v651 = 0; v651 < 7; v651 += 1) {	// L785
      for (int v652 = 0; v652 < 7; v652 += 1) {	// L786
        #pragma HLS pipeline II=1
        float v653 = v645[v650][v651][v652];	// L787
        v646[(v650 + (v647 * 8))][(v651 + (v648 * 7))][(v652 + (v649 * 7))] = v653;	// L788
      }
    }
  }
}

void forward_node54(
  float v654[8][7][7],
  float v655[8][7][7],
  float v656[8][8],
  float v657[8][7][7],
  float v658[8][7][7],
  int v659,
  int v660,
  int v661
) {	// L794
  #pragma HLS inline
  for (int v662 = 0; v662 < 8; v662 += 1) {	// L796
    for (int v663 = 0; v663 < 8; v663 += 2) {	// L797
      for (int v664 = 0; v664 < 7; v664 += 1) {	// L798
        for (int v665 = 0; v665 < 7; v665 += 1) {	// L799
          #pragma HLS pipeline II=1
          float v666 = v655[v663][v664][v665];	// L800
          float v667 = v657[v663][v664][v665];	// L801
          float v668 = v658[v663][v664][v665];	// L802
          float v669 = (v662 == 0) ? v667 : v668;	// L803
          float v670 = ((v662 + (v661 * 8)) == 0 && v659 == 0 && v660 == 0) ? v666 : v669;	// L804
          float v671 = v654[v662][v664][v665];	// L805
          float v672 = v656[v663][v662];	// L806
          float v673 = v671 * v672;	// L807
          float v674 = v670 + v673;	// L808
          bool v675 = v674 > (float)0.000000;	// L809
          float v676 = v675 ? v674 : (float)0.000000;	// L810
          float v677 = ((((-v662) + (v661 * -8)) + 255) == 0 && ((-v659) + 2) == 0 && ((-v660) + 2) == 0) ? v676 : v674;	// L811
          v658[v663][v664][v665] = v677;	// L812
          float v678 = v655[(v663 + 1)][v664][v665];	// L813
          float v679 = v657[(v663 + 1)][v664][v665];	// L814
          float v680 = v658[(v663 + 1)][v664][v665];	// L815
          float v681 = (v662 == 0) ? v679 : v680;	// L816
          float v682 = ((v662 + (v661 * 8)) == 0 && v659 == 0 && v660 == 0) ? v678 : v681;	// L817
          float v683 = v656[(v663 + 1)][v662];	// L818
          float v684 = v671 * v683;	// L819
          float v685 = v682 + v684;	// L820
          bool v686 = v685 > (float)0.000000;	// L821
          float v687 = v686 ? v685 : (float)0.000000;	// L822
          float v688 = ((((-v662) + (v661 * -8)) + 255) == 0 && ((-v659) + 2) == 0 && ((-v660) + 2) == 0) ? v687 : v685;	// L823
          v658[(v663 + 1)][v664][v665] = v688;	// L824
        }
      }
    }
  }
}

void forward_node55(
  float v689[256][256][3][3],
  float v690[8][8],
  int v691,
  int v692,
  int v693,
  int v694
) {	// L831
  #pragma HLS inline
  for (int v695 = 0; v695 < 8; v695 += 1) {	// L832
    for (int v696 = 0; v696 < 8; v696 += 1) {	// L833
      #pragma HLS pipeline II=1
      float v697 = v689[(v695 + (v693 * 8))][(v696 + (v694 * 8))][v691][v692];	// L834
      v690[v695][v696] = v697;	// L835
    }
  }
}

void forward_node56(
  float v698[256][14][14],
  float v699[8][7][7],
  int v700,
  int v701,
  int v702,
  int v703,
  int v704
) {	// L840
  #pragma HLS inline
  for (int v705 = 0; v705 < 8; v705 += 1) {	// L841
    for (int v706 = 0; v706 < 7; v706 += 1) {	// L842
      for (int v707 = 0; v707 < 7; v707 += 1) {	// L843
        #pragma HLS pipeline II=1
        float v708 = v698[(v705 + (v700 * 8))][(((v706 + v701) + (v702 * 7)) - 1)][(((v707 + v703) + (v704 * 7)) - 1)];	// L844
        v699[v705][v706][v707] = v708;	// L845
      }
    }
  }
}

void forward_node57(
  float v709[256][14][14],
  float v710[8][7][7],
  int v711,
  int v712,
  int v713
) {	// L851
  #pragma HLS inline
  for (int v714 = 0; v714 < 8; v714 += 1) {	// L852
    for (int v715 = 0; v715 < 7; v715 += 1) {	// L853
      for (int v716 = 0; v716 < 7; v716 += 1) {	// L854
        #pragma HLS pipeline II=1
        float v717 = v709[(v714 + (v711 * 8))][(v715 + (v712 * 7))][(v716 + (v713 * 7))];	// L855
        v710[v714][v715][v716] = v717;	// L856
      }
    }
  }
}

void forward_node58(
  float v718[256][14][14],
  float v719[8][7][7],
  int v720,
  int v721,
  int v722
) {	// L862
  #pragma HLS inline
  for (int v723 = 0; v723 < 8; v723 += 1) {	// L863
    for (int v724 = 0; v724 < 7; v724 += 1) {	// L864
      for (int v725 = 0; v725 < 7; v725 += 1) {	// L865
        #pragma HLS pipeline II=1
        float v726 = v718[(v723 + (v720 * 8))][(v724 + (v721 * 7))][(v725 + (v722 * 7))];	// L866
        v719[v723][v724][v725] = v726;	// L867
      }
    }
  }
}

void forward_node52(
  float v727[256][256][3][3],
  float v728[256][14][14],
  float v729[256][14][14],
  float v730[256][14][14],
  float v731[256][14][14]
) {	// L873
  for (int v732 = 0; v732 < 36864; v732 += 1) {	// L874
    #pragma HLS dataflow
    int v733 = (v732 % 2);	// L875
    int v734 = ((v732 / 2) % 2);	// L876
    int v735 = (((v732 / 2) / 2) % 32);	// L877
    int v736 = ((((v732 / 2) / 2) / 32) % 3);	// L878
    int v737 = (((((v732 / 2) / 2) / 32) / 3) % 3);	// L879
    int v738 = (((((v732 / 2) / 2) / 32) / 3) / 3);	// L880
    float v739[8][8];	// L881
    #pragma HLS array_partition variable=v739 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v739 type=ram_2p impl=lutram

    float v740[8][7][7];	// L882
    #pragma HLS bind_storage variable=v740 type=ram_t2p impl=bram

    float v741[8][7][7];	// L883
    #pragma HLS array_partition variable=v741 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v741 type=ram_2p impl=lutram

    float v742[8][7][7];	// L884
    #pragma HLS array_partition variable=v742 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v742 type=ram_2p impl=lutram

    forward_node58(v728, v742, v735, v734, v733);	// L885
    forward_node57(v730, v741, v735, v734, v733);	// L886
    forward_node56(v729, v740, v738, v737, v734, v736, v733);	// L887
    forward_node55(v727, v739, v737, v736, v735, v738);	// L888
    float v743[8][7][7];	// L889
    #pragma HLS array_partition variable=v743 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v743 type=ram_2p impl=lutram

    forward_node54(v740, v742, v739, v741, v743, v737, v736, v738);	// L890
    forward_node53(v743, v731, v735, v734, v733);	// L891
  }
}

void forward_node60(
  float v744[8][7][7],
  float v745[256][14][14],
  int v746,
  int v747,
  int v748
) {	// L895
  #pragma HLS inline
  for (int v749 = 0; v749 < 8; v749 += 1) {	// L896
    for (int v750 = 0; v750 < 7; v750 += 1) {	// L897
      for (int v751 = 0; v751 < 7; v751 += 1) {	// L898
        #pragma HLS pipeline II=1
        float v752 = v744[v749][v750][v751];	// L899
        v745[(v749 + (v746 * 8))][(v750 + (v747 * 7))][(v751 + (v748 * 7))] = v752;	// L900
      }
    }
  }
}

void forward_node61(
  float v753[8][7][7],
  float v754[256][14][14],
  int v755,
  int v756,
  int v757
) {	// L906
  #pragma HLS inline
  for (int v758 = 0; v758 < 8; v758 += 1) {	// L907
    for (int v759 = 0; v759 < 7; v759 += 1) {	// L908
      for (int v760 = 0; v760 < 7; v760 += 1) {	// L909
        #pragma HLS pipeline II=1
        float v761 = v753[v758][v759][v760];	// L910
        v754[(v758 + (v755 * 8))][(v759 + (v756 * 7))][(v760 + (v757 * 7))] = v761;	// L911
      }
    }
  }
}

void forward_node62(
  float v762[8][7][7],
  float v763[8][8],
  float v764[8][7][7],
  float v765[8][7][7],
  float v766[8][7][7],
  float v767[8][7][7],
  float v768[8][7][7],
  int v769
) {	// L917
  #pragma HLS inline
  for (int v770 = 0; v770 < 8; v770 += 1) {	// L919
    for (int v771 = 0; v771 < 8; v771 += 2) {	// L920
      for (int v772 = 0; v772 < 7; v772 += 1) {	// L921
        for (int v773 = 0; v773 < 7; v773 += 1) {	// L922
          #pragma HLS pipeline II=1
          float v774 = v765[v771][v772][v773];	// L923
          float v775 = v766[v771][v772][v773];	// L924
          float v776 = v768[v771][v772][v773];	// L925
          float v777 = (v770 == 0) ? v775 : v776;	// L926
          float v778 = ((v770 + (v769 * 8)) == 0) ? v774 : v777;	// L927
          float v779 = v762[v770][v772][v773];	// L928
          float v780 = v763[v771][v770];	// L929
          float v781 = v779 * v780;	// L930
          float v782 = v778 + v781;	// L931
          v768[v771][v772][v773] = v782;	// L932
          float v783 = v764[v771][v772][v773];	// L933
          float v784 = v783 + v782;	// L934
          bool v785 = v784 > (float)0.000000;	// L935
          float v786 = v785 ? v784 : (float)0.000000;	// L936
          if ((((-v770) + (v769 * -8)) + 127) == 0) {	// L937
            v767[v771][v772][v773] = v786;	// L938
          }
          float v787 = v765[(v771 + 1)][v772][v773];	// L940
          float v788 = v766[(v771 + 1)][v772][v773];	// L941
          float v789 = v768[(v771 + 1)][v772][v773];	// L942
          float v790 = (v770 == 0) ? v788 : v789;	// L943
          float v791 = ((v770 + (v769 * 8)) == 0) ? v787 : v790;	// L944
          float v792 = v763[(v771 + 1)][v770];	// L945
          float v793 = v779 * v792;	// L946
          float v794 = v791 + v793;	// L947
          v768[(v771 + 1)][v772][v773] = v794;	// L948
          float v795 = v764[(v771 + 1)][v772][v773];	// L949
          float v796 = v795 + v794;	// L950
          bool v797 = v796 > (float)0.000000;	// L951
          float v798 = v797 ? v796 : (float)0.000000;	// L952
          if ((((-v770) + (v769 * -8)) + 127) == 0) {	// L953
            v767[(v771 + 1)][v772][v773] = v798;	// L954
          }
        }
      }
    }
  }
}

void forward_node63(
  float v799[256][14][14],
  float v800[8][7][7],
  int v801,
  int v802,
  int v803
) {	// L962
  #pragma HLS inline
  for (int v804 = 0; v804 < 8; v804 += 1) {	// L963
    for (int v805 = 0; v805 < 7; v805 += 1) {	// L964
      for (int v806 = 0; v806 < 7; v806 += 1) {	// L965
        #pragma HLS pipeline II=1
        float v807 = v799[(v804 + (v801 * 8))][(v805 + (v802 * 7))][(v806 + (v803 * 7))];	// L966
        v800[v804][v805][v806] = v807;	// L967
      }
    }
  }
}

void forward_node64(
  float v808[256][128],
  float v809[8][8],
  int v810,
  int v811
) {	// L973
  #pragma HLS inline
  for (int v812 = 0; v812 < 8; v812 += 1) {	// L974
    for (int v813 = 0; v813 < 8; v813 += 1) {	// L975
      #pragma HLS pipeline II=1
      float v814 = v808[(v812 + (v810 * 8))][(v813 + (v811 * 8))];	// L976
      v809[v812][v813] = v814;	// L977
    }
  }
}

void forward_node65(
  float v815[128][28][28],
  float v816[8][7][7],
  int v817,
  int v818,
  int v819
) {	// L982
  #pragma HLS inline
  for (int v820 = 0; v820 < 8; v820 += 1) {	// L983
    for (int v821 = 0; v821 < 7; v821 += 1) {	// L984
      for (int v822 = 0; v822 < 7; v822 += 1) {	// L985
        #pragma HLS pipeline II=1
        float v823 = v815[(v820 + (v817 * 8))][((v821 * 2) + (v818 * 14))][((v822 * 2) + (v819 * 14))];	// L986
        v816[v820][v821][v822] = v823;	// L987
      }
    }
  }
}

void forward_node66(
  float v824[256][14][14],
  float v825[8][7][7],
  int v826,
  int v827,
  int v828
) {	// L993
  #pragma HLS inline
  for (int v829 = 0; v829 < 8; v829 += 1) {	// L994
    for (int v830 = 0; v830 < 7; v830 += 1) {	// L995
      for (int v831 = 0; v831 < 7; v831 += 1) {	// L996
        #pragma HLS pipeline II=1
        float v832 = v824[(v829 + (v826 * 8))][(v830 + (v827 * 7))][(v831 + (v828 * 7))];	// L997
        v825[v829][v830][v831] = v832;	// L998
      }
    }
  }
}

void forward_node67(
  float v833[256][14][14],
  float v834[8][7][7],
  int v835,
  int v836,
  int v837
) {	// L1004
  #pragma HLS inline
  for (int v838 = 0; v838 < 8; v838 += 1) {	// L1005
    for (int v839 = 0; v839 < 7; v839 += 1) {	// L1006
      for (int v840 = 0; v840 < 7; v840 += 1) {	// L1007
        #pragma HLS pipeline II=1
        float v841 = v833[(v838 + (v835 * 8))][(v839 + (v836 * 7))][(v840 + (v837 * 7))];	// L1008
        v834[v838][v839][v840] = v841;	// L1009
      }
    }
  }
}

void forward_node59(
  float v842[256][128],
  float v843[256][14][14],
  float v844[128][28][28],
  float v845[256][14][14],
  float v846[256][14][14],
  float v847[256][14][14],
  float v848[256][14][14]
) {	// L1015
  for (int v849 = 0; v849 < 2048; v849 += 1) {	// L1016
    #pragma HLS dataflow
    int v850 = (v849 % 2);	// L1017
    int v851 = ((v849 / 2) % 2);	// L1018
    int v852 = (((v849 / 2) / 2) % 32);	// L1019
    int v853 = (((v849 / 2) / 2) / 32);	// L1020
    float v854[8][7][7];	// L1021
    #pragma HLS array_partition variable=v854 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v854 type=ram_2p impl=lutram

    float v855[8][7][7];	// L1022
    #pragma HLS array_partition variable=v855 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v855 type=ram_2p impl=lutram

    float v856[8][8];	// L1023
    #pragma HLS array_partition variable=v856 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v856 type=ram_2p impl=lutram

    float v857[8][7][7];	// L1024
    #pragma HLS bind_storage variable=v857 type=ram_t2p impl=bram

    float v858[8][7][7];	// L1025
    #pragma HLS array_partition variable=v858 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v858 type=ram_2p impl=lutram

    float v859[8][7][7];	// L1026
    #pragma HLS array_partition variable=v859 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v859 type=ram_2p impl=lutram

    forward_node67(v843, v859, v852, v851, v850);	// L1027
    forward_node66(v846, v858, v852, v851, v850);	// L1028
    forward_node65(v844, v857, v853, v851, v850);	// L1029
    forward_node64(v842, v856, v852, v853);	// L1030
    forward_node63(v845, v855, v852, v851, v850);	// L1031
    float v860[8][7][7];	// L1032
    #pragma HLS array_partition variable=v860 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v860 type=ram_2p impl=lutram

    forward_node62(v857, v856, v855, v859, v858, v854, v860, v853);	// L1033
    forward_node61(v860, v847, v852, v851, v850);	// L1034
    forward_node60(v854, v848, v852, v851, v850);	// L1035
  }
}

void forward_node69(
  float v861[8][7][7],
  float v862[256][14][14],
  int v863,
  int v864,
  int v865
) {	// L1039
  #pragma HLS inline
  for (int v866 = 0; v866 < 8; v866 += 1) {	// L1040
    for (int v867 = 0; v867 < 7; v867 += 1) {	// L1041
      for (int v868 = 0; v868 < 7; v868 += 1) {	// L1042
        #pragma HLS pipeline II=1
        float v869 = v861[v866][v867][v868];	// L1043
        v862[(v866 + (v863 * 8))][(v867 + (v864 * 7))][(v868 + (v865 * 7))] = v869;	// L1044
      }
    }
  }
}

void forward_node70(
  float v870[8][7][7],
  float v871[8][7][7],
  float v872[8][8],
  float v873[8][7][7],
  float v874[8][7][7],
  int v875,
  int v876,
  int v877
) {	// L1050
  #pragma HLS inline
  for (int v878 = 0; v878 < 8; v878 += 1) {	// L1051
    for (int v879 = 0; v879 < 8; v879 += 2) {	// L1052
      for (int v880 = 0; v880 < 7; v880 += 1) {	// L1053
        for (int v881 = 0; v881 < 7; v881 += 1) {	// L1054
          #pragma HLS pipeline II=1
          float v882 = v870[v879][v880][v881];	// L1055
          float v883 = v873[v879][v880][v881];	// L1056
          float v884 = v874[v879][v880][v881];	// L1057
          float v885 = (v878 == 0) ? v883 : v884;	// L1058
          float v886 = ((v878 + (v877 * 8)) == 0 && v875 == 0 && v876 == 0) ? v882 : v885;	// L1059
          float v887 = v871[v878][v880][v881];	// L1060
          float v888 = v872[v879][v878];	// L1061
          float v889 = v887 * v888;	// L1062
          float v890 = v886 + v889;	// L1063
          v874[v879][v880][v881] = v890;	// L1064
          float v891 = v870[(v879 + 1)][v880][v881];	// L1065
          float v892 = v873[(v879 + 1)][v880][v881];	// L1066
          float v893 = v874[(v879 + 1)][v880][v881];	// L1067
          float v894 = (v878 == 0) ? v892 : v893;	// L1068
          float v895 = ((v878 + (v877 * 8)) == 0 && v875 == 0 && v876 == 0) ? v891 : v894;	// L1069
          float v896 = v872[(v879 + 1)][v878];	// L1070
          float v897 = v887 * v896;	// L1071
          float v898 = v895 + v897;	// L1072
          v874[(v879 + 1)][v880][v881] = v898;	// L1073
        }
      }
    }
  }
}

void forward_node71(
  float v899[256][256][3][3],
  float v900[8][8],
  int v901,
  int v902,
  int v903,
  int v904
) {	// L1080
  #pragma HLS inline
  for (int v905 = 0; v905 < 8; v905 += 1) {	// L1081
    for (int v906 = 0; v906 < 8; v906 += 1) {	// L1082
      #pragma HLS pipeline II=1
      float v907 = v899[(v905 + (v903 * 8))][(v906 + (v904 * 8))][v901][v902];	// L1083
      v900[v905][v906] = v907;	// L1084
    }
  }
}

void forward_node72(
  float v908[256][14][14],
  float v909[8][7][7],
  int v910,
  int v911,
  int v912,
  int v913,
  int v914
) {	// L1089
  #pragma HLS inline
  for (int v915 = 0; v915 < 8; v915 += 1) {	// L1090
    for (int v916 = 0; v916 < 7; v916 += 1) {	// L1091
      for (int v917 = 0; v917 < 7; v917 += 1) {	// L1092
        #pragma HLS pipeline II=1
        float v918 = v908[(v915 + (v910 * 8))][(((v916 + v911) + (v912 * 7)) - 1)][(((v917 + v913) + (v914 * 7)) - 1)];	// L1093
        v909[v915][v916][v917] = v918;	// L1094
      }
    }
  }
}

void forward_node73(
  float v919[256][14][14],
  float v920[8][7][7],
  int v921,
  int v922,
  int v923
) {	// L1100
  #pragma HLS inline
  for (int v924 = 0; v924 < 8; v924 += 1) {	// L1101
    for (int v925 = 0; v925 < 7; v925 += 1) {	// L1102
      for (int v926 = 0; v926 < 7; v926 += 1) {	// L1103
        #pragma HLS pipeline II=1
        float v927 = v919[(v924 + (v921 * 8))][(v925 + (v922 * 7))][(v926 + (v923 * 7))];	// L1104
        v920[v924][v925][v926] = v927;	// L1105
      }
    }
  }
}

void forward_node74(
  float v928[256][14][14],
  float v929[8][7][7],
  int v930,
  int v931,
  int v932
) {	// L1111
  #pragma HLS inline
  for (int v933 = 0; v933 < 8; v933 += 1) {	// L1112
    for (int v934 = 0; v934 < 7; v934 += 1) {	// L1113
      for (int v935 = 0; v935 < 7; v935 += 1) {	// L1114
        #pragma HLS pipeline II=1
        float v936 = v928[(v933 + (v930 * 8))][(v934 + (v931 * 7))][(v935 + (v932 * 7))];	// L1115
        v929[v933][v934][v935] = v936;	// L1116
      }
    }
  }
}

void forward_node68(
  float v937[256][14][14],
  float v938[256][256][3][3],
  float v939[256][14][14],
  float v940[256][14][14],
  float v941[256][14][14]
) {	// L1122
  for (int v942 = 0; v942 < 36864; v942 += 1) {	// L1123
    #pragma HLS dataflow
    int v943 = (v942 % 2);	// L1124
    int v944 = ((v942 / 2) % 2);	// L1125
    int v945 = (((v942 / 2) / 2) % 32);	// L1126
    int v946 = ((((v942 / 2) / 2) / 32) % 3);	// L1127
    int v947 = (((((v942 / 2) / 2) / 32) / 3) % 3);	// L1128
    int v948 = (((((v942 / 2) / 2) / 32) / 3) / 3);	// L1129
    float v949[8][8];	// L1130
    #pragma HLS array_partition variable=v949 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v949 type=ram_2p impl=lutram

    float v950[8][7][7];	// L1131
    #pragma HLS bind_storage variable=v950 type=ram_t2p impl=bram

    float v951[8][7][7];	// L1132
    #pragma HLS array_partition variable=v951 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v951 type=ram_2p impl=lutram

    float v952[8][7][7];	// L1133
    #pragma HLS array_partition variable=v952 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v952 type=ram_2p impl=lutram

    forward_node74(v939, v952, v945, v944, v943);	// L1134
    forward_node73(v940, v951, v945, v944, v943);	// L1135
    forward_node72(v937, v950, v948, v947, v944, v946, v943);	// L1136
    forward_node71(v938, v949, v947, v946, v945, v948);	// L1137
    float v953[8][7][7];	// L1138
    #pragma HLS array_partition variable=v953 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v953 type=ram_2p impl=lutram

    forward_node70(v952, v950, v949, v951, v953, v947, v946, v948);	// L1139
    forward_node69(v953, v941, v945, v944, v943);	// L1140
  }
}

void forward_node76(
  float v954[8][7][7],
  float v955[256][14][14],
  int v956,
  int v957,
  int v958
) {	// L1144
  #pragma HLS inline
  for (int v959 = 0; v959 < 8; v959 += 1) {	// L1145
    for (int v960 = 0; v960 < 7; v960 += 1) {	// L1146
      for (int v961 = 0; v961 < 7; v961 += 1) {	// L1147
        #pragma HLS pipeline II=1
        float v962 = v954[v959][v960][v961];	// L1148
        v955[(v959 + (v956 * 8))][(v960 + (v957 * 7))][(v961 + (v958 * 7))] = v962;	// L1149
      }
    }
  }
}

void forward_node77(
  float v963[8][7][7],
  float v964[8][7][7],
  float v965[8][8],
  float v966[8][7][7],
  float v967[8][7][7],
  int v968,
  int v969,
  int v970
) {	// L1155
  #pragma HLS inline
  for (int v971 = 0; v971 < 8; v971 += 1) {	// L1157
    for (int v972 = 0; v972 < 8; v972 += 2) {	// L1158
      for (int v973 = 0; v973 < 7; v973 += 1) {	// L1159
        for (int v974 = 0; v974 < 7; v974 += 1) {	// L1160
          #pragma HLS pipeline II=1
          float v975 = v963[v972][v973][v974];	// L1161
          float v976 = v966[v972][v973][v974];	// L1162
          float v977 = v967[v972][v973][v974];	// L1163
          float v978 = (v971 == 0) ? v976 : v977;	// L1164
          float v979 = ((v971 + (v968 * 8)) == 0 && v970 == 0 && v969 == 0) ? v975 : v978;	// L1165
          float v980 = v964[v971][v973][v974];	// L1166
          float v981 = v965[v972][v971];	// L1167
          float v982 = v980 * v981;	// L1168
          float v983 = v979 + v982;	// L1169
          bool v984 = v983 > (float)0.000000;	// L1170
          float v985 = v984 ? v983 : (float)0.000000;	// L1171
          float v986 = ((((-v971) + (v968 * -8)) + 127) == 0 && ((-v970) + 2) == 0 && ((-v969) + 2) == 0) ? v985 : v983;	// L1172
          v967[v972][v973][v974] = v986;	// L1173
          float v987 = v963[(v972 + 1)][v973][v974];	// L1174
          float v988 = v966[(v972 + 1)][v973][v974];	// L1175
          float v989 = v967[(v972 + 1)][v973][v974];	// L1176
          float v990 = (v971 == 0) ? v988 : v989;	// L1177
          float v991 = ((v971 + (v968 * 8)) == 0 && v970 == 0 && v969 == 0) ? v987 : v990;	// L1178
          float v992 = v965[(v972 + 1)][v971];	// L1179
          float v993 = v980 * v992;	// L1180
          float v994 = v991 + v993;	// L1181
          bool v995 = v994 > (float)0.000000;	// L1182
          float v996 = v995 ? v994 : (float)0.000000;	// L1183
          float v997 = ((((-v971) + (v968 * -8)) + 127) == 0 && ((-v970) + 2) == 0 && ((-v969) + 2) == 0) ? v996 : v994;	// L1184
          v967[(v972 + 1)][v973][v974] = v997;	// L1185
        }
      }
    }
  }
}

void forward_node78(
  float v998[256][128][3][3],
  float v999[8][8],
  int v1000,
  int v1001,
  int v1002,
  int v1003
) {	// L1192
  #pragma HLS inline
  for (int v1004 = 0; v1004 < 8; v1004 += 1) {	// L1193
    for (int v1005 = 0; v1005 < 8; v1005 += 1) {	// L1194
      #pragma HLS pipeline II=1
      float v1006 = v998[(v1004 + (v1002 * 8))][(v1005 + (v1003 * 8))][v1000][v1001];	// L1195
      v999[v1004][v1005] = v1006;	// L1196
    }
  }
}

void forward_node79(
  float v1007[128][28][28],
  float v1008[8][7][7],
  int v1009,
  int v1010,
  int v1011,
  int v1012,
  int v1013
) {	// L1201
  #pragma HLS inline
  for (int v1014 = 0; v1014 < 8; v1014 += 1) {	// L1202
    for (int v1015 = 0; v1015 < 7; v1015 += 1) {	// L1203
      for (int v1016 = 0; v1016 < 7; v1016 += 1) {	// L1204
        #pragma HLS pipeline II=1
        float v1017 = v1007[(v1014 + (v1009 * 8))][((((v1015 * 2) + v1010) + (v1011 * 14)) - 1)][((((v1016 * 2) + v1012) + (v1013 * 14)) - 1)];	// L1205
        v1008[v1014][v1015][v1016] = v1017;	// L1206
      }
    }
  }
}

void forward_node80(
  float v1018[256][14][14],
  float v1019[8][7][7],
  int v1020,
  int v1021,
  int v1022
) {	// L1212
  #pragma HLS inline
  for (int v1023 = 0; v1023 < 8; v1023 += 1) {	// L1213
    for (int v1024 = 0; v1024 < 7; v1024 += 1) {	// L1214
      for (int v1025 = 0; v1025 < 7; v1025 += 1) {	// L1215
        #pragma HLS pipeline II=1
        float v1026 = v1018[(v1023 + (v1020 * 8))][(v1024 + (v1021 * 7))][(v1025 + (v1022 * 7))];	// L1216
        v1019[v1023][v1024][v1025] = v1026;	// L1217
      }
    }
  }
}

void forward_node81(
  float v1027[256][14][14],
  float v1028[8][7][7],
  int v1029,
  int v1030,
  int v1031
) {	// L1223
  #pragma HLS inline
  for (int v1032 = 0; v1032 < 8; v1032 += 1) {	// L1224
    for (int v1033 = 0; v1033 < 7; v1033 += 1) {	// L1225
      for (int v1034 = 0; v1034 < 7; v1034 += 1) {	// L1226
        #pragma HLS pipeline II=1
        float v1035 = v1027[(v1032 + (v1029 * 8))][(v1033 + (v1030 * 7))][(v1034 + (v1031 * 7))];	// L1227
        v1028[v1032][v1033][v1034] = v1035;	// L1228
      }
    }
  }
}

void forward_node75(
  float v1036[256][128][3][3],
  float v1037[256][14][14],
  float v1038[128][28][28],
  float v1039[256][14][14],
  float v1040[256][14][14]
) {	// L1234
  for (int v1041 = 0; v1041 < 18432; v1041 += 1) {	// L1235
    #pragma HLS dataflow
    int v1042 = (v1041 % 2);	// L1236
    int v1043 = ((v1041 / 2) % 2);	// L1237
    int v1044 = (((v1041 / 2) / 2) % 32);	// L1238
    int v1045 = ((((v1041 / 2) / 2) / 32) % 3);	// L1239
    int v1046 = (((((v1041 / 2) / 2) / 32) / 3) % 3);	// L1240
    int v1047 = (((((v1041 / 2) / 2) / 32) / 3) / 3);	// L1241
    float v1048[8][8];	// L1242
    #pragma HLS array_partition variable=v1048 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v1048 type=ram_2p impl=lutram

    float v1049[8][7][7];	// L1243
    #pragma HLS bind_storage variable=v1049 type=ram_t2p impl=bram

    float v1050[8][7][7];	// L1244
    #pragma HLS array_partition variable=v1050 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v1050 type=ram_2p impl=lutram

    float v1051[8][7][7];	// L1245
    #pragma HLS array_partition variable=v1051 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v1051 type=ram_2p impl=lutram

    forward_node81(v1037, v1051, v1044, v1043, v1042);	// L1246
    forward_node80(v1039, v1050, v1044, v1043, v1042);	// L1247
    forward_node79(v1038, v1049, v1047, v1046, v1043, v1045, v1042);	// L1248
    forward_node78(v1036, v1048, v1046, v1045, v1044, v1047);	// L1249
    float v1052[8][7][7];	// L1250
    #pragma HLS array_partition variable=v1052 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v1052 type=ram_2p impl=lutram

    forward_node77(v1051, v1049, v1048, v1050, v1052, v1047, v1045, v1046);	// L1251
    forward_node76(v1052, v1040, v1044, v1043, v1042);	// L1252
  }
}

void forward_node83(
  float v1053[8][7][7],
  float v1054[128][28][28],
  int v1055,
  int v1056,
  int v1057
) {	// L1256
  #pragma HLS inline
  for (int v1058 = 0; v1058 < 8; v1058 += 1) {	// L1257
    for (int v1059 = 0; v1059 < 7; v1059 += 1) {	// L1258
      for (int v1060 = 0; v1060 < 7; v1060 += 1) {	// L1259
        #pragma HLS pipeline II=1
        float v1061 = v1053[v1058][v1059][v1060];	// L1260
        v1054[(v1058 + (v1055 * 8))][(v1059 + (v1056 * 7))][(v1060 + (v1057 * 7))] = v1061;	// L1261
      }
    }
  }
}

void forward_node84(
  float v1062[8][7][7],
  float v1063[128][28][28],
  int v1064,
  int v1065,
  int v1066
) {	// L1267
  #pragma HLS inline
  for (int v1067 = 0; v1067 < 8; v1067 += 1) {	// L1268
    for (int v1068 = 0; v1068 < 7; v1068 += 1) {	// L1269
      for (int v1069 = 0; v1069 < 7; v1069 += 1) {	// L1270
        #pragma HLS pipeline II=1
        float v1070 = v1062[v1067][v1068][v1069];	// L1271
        v1063[(v1067 + (v1064 * 8))][(v1068 + (v1065 * 7))][(v1069 + (v1066 * 7))] = v1070;	// L1272
      }
    }
  }
}

void forward_node85(
  float v1071[8][7][7],
  float v1072[8][8],
  float v1073[8][7][7],
  float v1074[8][7][7],
  float v1075[8][7][7],
  float v1076[8][7][7],
  int v1077,
  int v1078,
  int v1079
) {	// L1278
  #pragma HLS inline
  for (int v1080 = 0; v1080 < 8; v1080 += 1) {	// L1280
    for (int v1081 = 0; v1081 < 8; v1081 += 2) {	// L1281
      for (int v1082 = 0; v1082 < 7; v1082 += 1) {	// L1282
        for (int v1083 = 0; v1083 < 7; v1083 += 1) {	// L1283
          #pragma HLS pipeline II=1
          float v1084 = v1071[v1080][v1082][v1083];	// L1284
          float v1085 = v1072[v1081][v1080];	// L1285
          float v1086 = v1074[v1081][v1082][v1083];	// L1286
          float v1087 = v1076[v1081][v1082][v1083];	// L1287
          float v1088 = (v1080 == 0) ? v1086 : v1087;	// L1288
          float v1089 = v1084 * v1085;	// L1289
          float v1090 = v1088 + v1089;	// L1290
          v1076[v1081][v1082][v1083] = v1090;	// L1291
          float v1091 = v1073[v1081][v1082][v1083];	// L1292
          float v1092 = v1090 + v1091;	// L1293
          bool v1093 = v1092 > (float)0.000000;	// L1294
          float v1094 = v1093 ? v1092 : (float)0.000000;	// L1295
          if ((((-v1080) + (v1077 * -8)) + 127) == 0 && ((-v1078) + 2) == 0 && ((-v1079) + 2) == 0) {	// L1296
            v1075[v1081][v1082][v1083] = v1094;	// L1297
          }
          float v1095 = v1072[(v1081 + 1)][v1080];	// L1299
          float v1096 = v1074[(v1081 + 1)][v1082][v1083];	// L1300
          float v1097 = v1076[(v1081 + 1)][v1082][v1083];	// L1301
          float v1098 = (v1080 == 0) ? v1096 : v1097;	// L1302
          float v1099 = v1084 * v1095;	// L1303
          float v1100 = v1098 + v1099;	// L1304
          v1076[(v1081 + 1)][v1082][v1083] = v1100;	// L1305
          float v1101 = v1073[(v1081 + 1)][v1082][v1083];	// L1306
          float v1102 = v1100 + v1101;	// L1307
          bool v1103 = v1102 > (float)0.000000;	// L1308
          float v1104 = v1103 ? v1102 : (float)0.000000;	// L1309
          if ((((-v1080) + (v1077 * -8)) + 127) == 0 && ((-v1078) + 2) == 0 && ((-v1079) + 2) == 0) {	// L1310
            v1075[(v1081 + 1)][v1082][v1083] = v1104;	// L1311
          }
        }
      }
    }
  }
}

void forward_node86(
  float v1105[128][28][28],
  float v1106[8][7][7],
  int v1107,
  int v1108,
  int v1109
) {	// L1319
  #pragma HLS inline
  for (int v1110 = 0; v1110 < 8; v1110 += 1) {	// L1320
    for (int v1111 = 0; v1111 < 7; v1111 += 1) {	// L1321
      for (int v1112 = 0; v1112 < 7; v1112 += 1) {	// L1322
        #pragma HLS pipeline II=1
        float v1113 = v1105[(v1110 + (v1107 * 8))][(v1111 + (v1108 * 7))][(v1112 + (v1109 * 7))];	// L1323
        v1106[v1110][v1111][v1112] = v1113;	// L1324
      }
    }
  }
}

void forward_node87(
  float v1114[128][28][28],
  float v1115[8][7][7],
  int v1116,
  int v1117,
  int v1118
) {	// L1330
  #pragma HLS inline
  for (int v1119 = 0; v1119 < 8; v1119 += 1) {	// L1331
    for (int v1120 = 0; v1120 < 7; v1120 += 1) {	// L1332
      for (int v1121 = 0; v1121 < 7; v1121 += 1) {	// L1333
        #pragma HLS pipeline II=1
        float v1122 = v1114[(v1119 + (v1116 * 8))][(v1120 + (v1117 * 7))][(v1121 + (v1118 * 7))];	// L1334
        v1115[v1119][v1120][v1121] = v1122;	// L1335
      }
    }
  }
}

void forward_node88(
  float v1123[128][128][3][3],
  float v1124[8][8],
  int v1125,
  int v1126,
  int v1127,
  int v1128
) {	// L1341
  #pragma HLS inline
  for (int v1129 = 0; v1129 < 8; v1129 += 1) {	// L1342
    for (int v1130 = 0; v1130 < 8; v1130 += 1) {	// L1343
      #pragma HLS pipeline II=1
      float v1131 = v1123[(v1129 + (v1127 * 8))][(v1130 + (v1128 * 8))][v1125][v1126];	// L1344
      v1124[v1129][v1130] = v1131;	// L1345
    }
  }
}

void forward_node89(
  float v1132[128][28][28],
  float v1133[8][7][7],
  int v1134,
  int v1135,
  int v1136,
  int v1137,
  int v1138
) {	// L1350
  #pragma HLS inline
  for (int v1139 = 0; v1139 < 8; v1139 += 1) {	// L1351
    for (int v1140 = 0; v1140 < 7; v1140 += 1) {	// L1352
      for (int v1141 = 0; v1141 < 7; v1141 += 1) {	// L1353
        #pragma HLS pipeline II=1
        float v1142 = v1132[(v1139 + (v1134 * 8))][(((v1140 + v1135) + (v1136 * 7)) - 1)][(((v1141 + v1137) + (v1138 * 7)) - 1)];	// L1354
        v1133[v1139][v1140][v1141] = v1142;	// L1355
      }
    }
  }
}

void forward_node82(
  float v1143[128][28][28],
  float v1144[128][28][28],
  float v1145[128][128][3][3],
  float v1146[128][28][28],
  float v1147[128][28][28],
  float v1148[128][28][28]
) {	// L1361
  for (int v1149 = 0; v1149 < 36864; v1149 += 1) {	// L1362
    #pragma HLS dataflow
    int v1150 = (v1149 % 4);	// L1363
    int v1151 = ((v1149 / 4) % 4);	// L1364
    int v1152 = (((v1149 / 4) / 4) % 16);	// L1365
    int v1153 = ((((v1149 / 4) / 4) / 16) % 3);	// L1366
    int v1154 = (((((v1149 / 4) / 4) / 16) / 3) % 3);	// L1367
    int v1155 = (((((v1149 / 4) / 4) / 16) / 3) / 3);	// L1368
    float v1156[8][7][7];	// L1369
    #pragma HLS array_partition variable=v1156 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v1156 type=ram_2p impl=lutram

    float v1157[8][7][7];	// L1370
    #pragma HLS array_partition variable=v1157 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v1157 type=ram_2p impl=lutram

    float v1158[8][7][7];	// L1371
    #pragma HLS array_partition variable=v1158 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v1158 type=ram_2p impl=lutram

    float v1159[8][8];	// L1372
    #pragma HLS array_partition variable=v1159 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v1159 type=ram_2p impl=lutram

    float v1160[8][7][7];	// L1373
    #pragma HLS bind_storage variable=v1160 type=ram_t2p impl=bram

    forward_node89(v1144, v1160, v1155, v1154, v1151, v1153, v1150);	// L1374
    forward_node88(v1145, v1159, v1154, v1153, v1152, v1155);	// L1375
    forward_node87(v1146, v1158, v1152, v1151, v1150);	// L1376
    forward_node86(v1143, v1157, v1152, v1151, v1150);	// L1377
    float v1161[8][7][7];	// L1378
    #pragma HLS array_partition variable=v1161 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v1161 type=ram_2p impl=lutram

    forward_node85(v1160, v1159, v1157, v1158, v1156, v1161, v1155, v1154, v1153);	// L1379
    forward_node84(v1161, v1147, v1152, v1151, v1150);	// L1380
    forward_node83(v1156, v1148, v1152, v1151, v1150);	// L1381
  }
}

void forward_node91(
  float v1162[8][7][7],
  float v1163[128][28][28],
  int v1164,
  int v1165,
  int v1166
) {	// L1385
  #pragma HLS inline
  for (int v1167 = 0; v1167 < 8; v1167 += 1) {	// L1386
    for (int v1168 = 0; v1168 < 7; v1168 += 1) {	// L1387
      for (int v1169 = 0; v1169 < 7; v1169 += 1) {	// L1388
        #pragma HLS pipeline II=1
        float v1170 = v1162[v1167][v1168][v1169];	// L1389
        v1163[(v1167 + (v1164 * 8))][(v1168 + (v1165 * 7))][(v1169 + (v1166 * 7))] = v1170;	// L1390
      }
    }
  }
}

void forward_node92(
  float v1171[8][8],
  float v1172[8][7][7],
  float v1173[8][7][7],
  float v1174[8][7][7],
  float v1175[8][7][7],
  int v1176,
  int v1177,
  int v1178
) {	// L1396
  #pragma HLS inline
  for (int v1179 = 0; v1179 < 8; v1179 += 1) {	// L1398
    for (int v1180 = 0; v1180 < 8; v1180 += 2) {	// L1399
      for (int v1181 = 0; v1181 < 7; v1181 += 1) {	// L1400
        for (int v1182 = 0; v1182 < 7; v1182 += 1) {	// L1401
          #pragma HLS pipeline II=1
          float v1183 = v1173[v1180][v1181][v1182];	// L1402
          float v1184 = v1174[v1180][v1181][v1182];	// L1403
          float v1185 = v1175[v1180][v1181][v1182];	// L1404
          float v1186 = (v1179 == 0) ? v1184 : v1185;	// L1405
          float v1187 = ((v1179 + (v1176 * 8)) == 0 && v1178 == 0 && v1177 == 0) ? v1183 : v1186;	// L1406
          float v1188 = v1172[v1179][v1181][v1182];	// L1407
          float v1189 = v1171[v1180][v1179];	// L1408
          float v1190 = v1188 * v1189;	// L1409
          float v1191 = v1187 + v1190;	// L1410
          bool v1192 = v1191 > (float)0.000000;	// L1411
          float v1193 = v1192 ? v1191 : (float)0.000000;	// L1412
          float v1194 = ((((-v1179) + (v1176 * -8)) + 127) == 0 && ((-v1178) + 2) == 0 && ((-v1177) + 2) == 0) ? v1193 : v1191;	// L1413
          v1175[v1180][v1181][v1182] = v1194;	// L1414
          float v1195 = v1173[(v1180 + 1)][v1181][v1182];	// L1415
          float v1196 = v1174[(v1180 + 1)][v1181][v1182];	// L1416
          float v1197 = v1175[(v1180 + 1)][v1181][v1182];	// L1417
          float v1198 = (v1179 == 0) ? v1196 : v1197;	// L1418
          float v1199 = ((v1179 + (v1176 * 8)) == 0 && v1178 == 0 && v1177 == 0) ? v1195 : v1198;	// L1419
          float v1200 = v1171[(v1180 + 1)][v1179];	// L1420
          float v1201 = v1188 * v1200;	// L1421
          float v1202 = v1199 + v1201;	// L1422
          bool v1203 = v1202 > (float)0.000000;	// L1423
          float v1204 = v1203 ? v1202 : (float)0.000000;	// L1424
          float v1205 = ((((-v1179) + (v1176 * -8)) + 127) == 0 && ((-v1178) + 2) == 0 && ((-v1177) + 2) == 0) ? v1204 : v1202;	// L1425
          v1175[(v1180 + 1)][v1181][v1182] = v1205;	// L1426
        }
      }
    }
  }
}

void forward_node93(
  float v1206[128][128][3][3],
  float v1207[8][8],
  int v1208,
  int v1209,
  int v1210,
  int v1211
) {	// L1433
  #pragma HLS inline
  for (int v1212 = 0; v1212 < 8; v1212 += 1) {	// L1434
    for (int v1213 = 0; v1213 < 8; v1213 += 1) {	// L1435
      #pragma HLS pipeline II=1
      float v1214 = v1206[(v1212 + (v1210 * 8))][(v1213 + (v1211 * 8))][v1208][v1209];	// L1436
      v1207[v1212][v1213] = v1214;	// L1437
    }
  }
}

void forward_node94(
  float v1215[128][28][28],
  float v1216[8][7][7],
  int v1217,
  int v1218,
  int v1219,
  int v1220,
  int v1221
) {	// L1442
  #pragma HLS inline
  for (int v1222 = 0; v1222 < 8; v1222 += 1) {	// L1443
    for (int v1223 = 0; v1223 < 7; v1223 += 1) {	// L1444
      for (int v1224 = 0; v1224 < 7; v1224 += 1) {	// L1445
        #pragma HLS pipeline II=1
        float v1225 = v1215[(v1222 + (v1217 * 8))][(((v1223 + v1218) + (v1219 * 7)) - 1)][(((v1224 + v1220) + (v1221 * 7)) - 1)];	// L1446
        v1216[v1222][v1223][v1224] = v1225;	// L1447
      }
    }
  }
}

void forward_node95(
  float v1226[128][28][28],
  float v1227[8][7][7],
  int v1228,
  int v1229,
  int v1230
) {	// L1453
  #pragma HLS inline
  for (int v1231 = 0; v1231 < 8; v1231 += 1) {	// L1454
    for (int v1232 = 0; v1232 < 7; v1232 += 1) {	// L1455
      for (int v1233 = 0; v1233 < 7; v1233 += 1) {	// L1456
        #pragma HLS pipeline II=1
        float v1234 = v1226[(v1231 + (v1228 * 8))][(v1232 + (v1229 * 7))][(v1233 + (v1230 * 7))];	// L1457
        v1227[v1231][v1232][v1233] = v1234;	// L1458
      }
    }
  }
}

void forward_node96(
  float v1235[128][28][28],
  float v1236[8][7][7],
  int v1237,
  int v1238,
  int v1239
) {	// L1464
  #pragma HLS inline
  for (int v1240 = 0; v1240 < 8; v1240 += 1) {	// L1465
    for (int v1241 = 0; v1241 < 7; v1241 += 1) {	// L1466
      for (int v1242 = 0; v1242 < 7; v1242 += 1) {	// L1467
        #pragma HLS pipeline II=1
        float v1243 = v1235[(v1240 + (v1237 * 8))][(v1241 + (v1238 * 7))][(v1242 + (v1239 * 7))];	// L1468
        v1236[v1240][v1241][v1242] = v1243;	// L1469
      }
    }
  }
}

void forward_node90(
  float v1244[128][128][3][3],
  float v1245[128][28][28],
  float v1246[128][28][28],
  float v1247[128][28][28],
  float v1248[128][28][28]
) {	// L1475
  for (int v1249 = 0; v1249 < 36864; v1249 += 1) {	// L1476
    #pragma HLS dataflow
    int v1250 = (v1249 % 4);	// L1477
    int v1251 = ((v1249 / 4) % 4);	// L1478
    int v1252 = (((v1249 / 4) / 4) % 16);	// L1479
    int v1253 = ((((v1249 / 4) / 4) / 16) % 3);	// L1480
    int v1254 = (((((v1249 / 4) / 4) / 16) / 3) % 3);	// L1481
    int v1255 = (((((v1249 / 4) / 4) / 16) / 3) / 3);	// L1482
    float v1256[8][8];	// L1483
    #pragma HLS array_partition variable=v1256 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v1256 type=ram_2p impl=lutram

    float v1257[8][7][7];	// L1484
    #pragma HLS bind_storage variable=v1257 type=ram_t2p impl=bram

    float v1258[8][7][7];	// L1485
    #pragma HLS array_partition variable=v1258 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v1258 type=ram_2p impl=lutram

    float v1259[8][7][7];	// L1486
    #pragma HLS array_partition variable=v1259 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v1259 type=ram_2p impl=lutram

    forward_node96(v1245, v1259, v1252, v1251, v1250);	// L1487
    forward_node95(v1247, v1258, v1252, v1251, v1250);	// L1488
    forward_node94(v1246, v1257, v1255, v1254, v1251, v1253, v1250);	// L1489
    forward_node93(v1244, v1256, v1254, v1253, v1252, v1255);	// L1490
    float v1260[8][7][7];	// L1491
    #pragma HLS array_partition variable=v1260 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v1260 type=ram_2p impl=lutram

    forward_node92(v1256, v1257, v1259, v1258, v1260, v1255, v1253, v1254);	// L1492
    forward_node91(v1260, v1248, v1252, v1251, v1250);	// L1493
  }
}

void forward_node98(
  float v1261[8][7][7],
  float v1262[128][28][28],
  int v1263,
  int v1264,
  int v1265
) {	// L1497
  #pragma HLS inline
  for (int v1266 = 0; v1266 < 8; v1266 += 1) {	// L1498
    for (int v1267 = 0; v1267 < 7; v1267 += 1) {	// L1499
      for (int v1268 = 0; v1268 < 7; v1268 += 1) {	// L1500
        #pragma HLS pipeline II=1
        float v1269 = v1261[v1266][v1267][v1268];	// L1501
        v1262[(v1266 + (v1263 * 8))][(v1267 + (v1264 * 7))][(v1268 + (v1265 * 7))] = v1269;	// L1502
      }
    }
  }
}

void forward_node99(
  float v1270[8][7][7],
  float v1271[128][28][28],
  int v1272,
  int v1273,
  int v1274
) {	// L1508
  #pragma HLS inline
  for (int v1275 = 0; v1275 < 8; v1275 += 1) {	// L1509
    for (int v1276 = 0; v1276 < 7; v1276 += 1) {	// L1510
      for (int v1277 = 0; v1277 < 7; v1277 += 1) {	// L1511
        #pragma HLS pipeline II=1
        float v1278 = v1270[v1275][v1276][v1277];	// L1512
        v1271[(v1275 + (v1272 * 8))][(v1276 + (v1273 * 7))][(v1277 + (v1274 * 7))] = v1278;	// L1513
      }
    }
  }
}

void forward_node100(
  float v1279[8][8],
  float v1280[8][7][7],
  float v1281[8][7][7],
  float v1282[8][7][7],
  float v1283[8][7][7],
  float v1284[8][7][7],
  float v1285[8][7][7],
  int v1286
) {	// L1519
  #pragma HLS inline
  for (int v1287 = 0; v1287 < 8; v1287 += 1) {	// L1521
    for (int v1288 = 0; v1288 < 8; v1288 += 2) {	// L1522
      for (int v1289 = 0; v1289 < 7; v1289 += 1) {	// L1523
        for (int v1290 = 0; v1290 < 7; v1290 += 1) {	// L1524
          #pragma HLS pipeline II=1
          float v1291 = v1280[v1288][v1289][v1290];	// L1525
          float v1292 = v1283[v1288][v1289][v1290];	// L1526
          float v1293 = v1284[v1288][v1289][v1290];	// L1527
          float v1294 = (v1287 == 0) ? v1292 : v1293;	// L1528
          float v1295 = ((v1287 + (v1286 * 8)) == 0) ? v1291 : v1294;	// L1529
          float v1296 = v1281[v1287][v1289][v1290];	// L1530
          float v1297 = v1279[v1288][v1287];	// L1531
          float v1298 = v1296 * v1297;	// L1532
          float v1299 = v1295 + v1298;	// L1533
          v1284[v1288][v1289][v1290] = v1299;	// L1534
          float v1300 = v1282[v1288][v1289][v1290];	// L1535
          float v1301 = v1300 + v1299;	// L1536
          bool v1302 = v1301 > (float)0.000000;	// L1537
          float v1303 = v1302 ? v1301 : (float)0.000000;	// L1538
          if ((((-v1287) + (v1286 * -8)) + 63) == 0) {	// L1539
            v1285[v1288][v1289][v1290] = v1303;	// L1540
          }
          float v1304 = v1280[(v1288 + 1)][v1289][v1290];	// L1542
          float v1305 = v1283[(v1288 + 1)][v1289][v1290];	// L1543
          float v1306 = v1284[(v1288 + 1)][v1289][v1290];	// L1544
          float v1307 = (v1287 == 0) ? v1305 : v1306;	// L1545
          float v1308 = ((v1287 + (v1286 * 8)) == 0) ? v1304 : v1307;	// L1546
          float v1309 = v1279[(v1288 + 1)][v1287];	// L1547
          float v1310 = v1296 * v1309;	// L1548
          float v1311 = v1308 + v1310;	// L1549
          v1284[(v1288 + 1)][v1289][v1290] = v1311;	// L1550
          float v1312 = v1282[(v1288 + 1)][v1289][v1290];	// L1551
          float v1313 = v1312 + v1311;	// L1552
          bool v1314 = v1313 > (float)0.000000;	// L1553
          float v1315 = v1314 ? v1313 : (float)0.000000;	// L1554
          if ((((-v1287) + (v1286 * -8)) + 63) == 0) {	// L1555
            v1285[(v1288 + 1)][v1289][v1290] = v1315;	// L1556
          }
        }
      }
    }
  }
}

void forward_node101(
  float v1316[128][28][28],
  float v1317[8][7][7],
  int v1318,
  int v1319,
  int v1320
) {	// L1564
  #pragma HLS inline
  for (int v1321 = 0; v1321 < 8; v1321 += 1) {	// L1565
    for (int v1322 = 0; v1322 < 7; v1322 += 1) {	// L1566
      for (int v1323 = 0; v1323 < 7; v1323 += 1) {	// L1567
        #pragma HLS pipeline II=1
        float v1324 = v1316[(v1321 + (v1318 * 8))][(v1322 + (v1319 * 7))][(v1323 + (v1320 * 7))];	// L1568
        v1317[v1321][v1322][v1323] = v1324;	// L1569
      }
    }
  }
}

void forward_node102(
  float v1325[128][64],
  float v1326[8][8],
  int v1327,
  int v1328
) {	// L1575
  #pragma HLS inline
  for (int v1329 = 0; v1329 < 8; v1329 += 1) {	// L1576
    for (int v1330 = 0; v1330 < 8; v1330 += 1) {	// L1577
      #pragma HLS pipeline II=1
      float v1331 = v1325[(v1329 + (v1327 * 8))][(v1330 + (v1328 * 8))];	// L1578
      v1326[v1329][v1330] = v1331;	// L1579
    }
  }
}

void forward_node103(
  float v1332[64][56][56],
  float v1333[8][7][7],
  int v1334,
  int v1335,
  int v1336
) {	// L1584
  #pragma HLS inline
  for (int v1337 = 0; v1337 < 8; v1337 += 1) {	// L1585
    for (int v1338 = 0; v1338 < 7; v1338 += 1) {	// L1586
      for (int v1339 = 0; v1339 < 7; v1339 += 1) {	// L1587
        #pragma HLS pipeline II=1
        float v1340 = v1332[(v1337 + (v1334 * 8))][((v1338 * 2) + (v1335 * 14))][((v1339 * 2) + (v1336 * 14))];	// L1588
        v1333[v1337][v1338][v1339] = v1340;	// L1589
      }
    }
  }
}

void forward_node104(
  float v1341[128][28][28],
  float v1342[8][7][7],
  int v1343,
  int v1344,
  int v1345
) {	// L1595
  #pragma HLS inline
  for (int v1346 = 0; v1346 < 8; v1346 += 1) {	// L1596
    for (int v1347 = 0; v1347 < 7; v1347 += 1) {	// L1597
      for (int v1348 = 0; v1348 < 7; v1348 += 1) {	// L1598
        #pragma HLS pipeline II=1
        float v1349 = v1341[(v1346 + (v1343 * 8))][(v1347 + (v1344 * 7))][(v1348 + (v1345 * 7))];	// L1599
        v1342[v1346][v1347][v1348] = v1349;	// L1600
      }
    }
  }
}

void forward_node105(
  float v1350[128][28][28],
  float v1351[8][7][7],
  int v1352,
  int v1353,
  int v1354
) {	// L1606
  #pragma HLS inline
  for (int v1355 = 0; v1355 < 8; v1355 += 1) {	// L1607
    for (int v1356 = 0; v1356 < 7; v1356 += 1) {	// L1608
      for (int v1357 = 0; v1357 < 7; v1357 += 1) {	// L1609
        #pragma HLS pipeline II=1
        float v1358 = v1350[(v1355 + (v1352 * 8))][(v1356 + (v1353 * 7))][(v1357 + (v1354 * 7))];	// L1610
        v1351[v1355][v1356][v1357] = v1358;	// L1611
      }
    }
  }
}

void forward_node97(
  float v1359[128][28][28],
  float v1360[128][64],
  float v1361[128][28][28],
  float v1362[64][56][56],
  float v1363[128][28][28],
  float v1364[128][28][28],
  float v1365[128][28][28]
) {	// L1617
  for (int v1366 = 0; v1366 < 2048; v1366 += 1) {	// L1618
    #pragma HLS dataflow
    int v1367 = (v1366 % 4);	// L1619
    int v1368 = ((v1366 / 4) % 4);	// L1620
    int v1369 = (((v1366 / 4) / 4) % 16);	// L1621
    int v1370 = (((v1366 / 4) / 4) / 16);	// L1622
    float v1371[8][7][7];	// L1623
    #pragma HLS array_partition variable=v1371 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v1371 type=ram_2p impl=lutram

    float v1372[8][7][7];	// L1624
    #pragma HLS array_partition variable=v1372 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v1372 type=ram_2p impl=lutram

    float v1373[8][8];	// L1625
    #pragma HLS array_partition variable=v1373 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v1373 type=ram_2p impl=lutram

    float v1374[8][7][7];	// L1626
    #pragma HLS bind_storage variable=v1374 type=ram_t2p impl=bram

    float v1375[8][7][7];	// L1627
    #pragma HLS array_partition variable=v1375 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v1375 type=ram_2p impl=lutram

    float v1376[8][7][7];	// L1628
    #pragma HLS array_partition variable=v1376 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v1376 type=ram_2p impl=lutram

    forward_node105(v1359, v1376, v1369, v1368, v1367);	// L1629
    forward_node104(v1363, v1375, v1369, v1368, v1367);	// L1630
    forward_node103(v1362, v1374, v1370, v1368, v1367);	// L1631
    forward_node102(v1360, v1373, v1369, v1370);	// L1632
    forward_node101(v1361, v1372, v1369, v1368, v1367);	// L1633
    float v1377[8][7][7];	// L1634
    #pragma HLS array_partition variable=v1377 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v1377 type=ram_2p impl=lutram

    forward_node100(v1373, v1376, v1374, v1372, v1375, v1377, v1371, v1370);	// L1635
    forward_node99(v1377, v1365, v1369, v1368, v1367);	// L1636
    forward_node98(v1371, v1364, v1369, v1368, v1367);	// L1637
  }
}

void forward_node107(
  float v1378[8][7][7],
  float v1379[128][28][28],
  int v1380,
  int v1381,
  int v1382
) {	// L1641
  #pragma HLS inline
  for (int v1383 = 0; v1383 < 8; v1383 += 1) {	// L1642
    for (int v1384 = 0; v1384 < 7; v1384 += 1) {	// L1643
      for (int v1385 = 0; v1385 < 7; v1385 += 1) {	// L1644
        #pragma HLS pipeline II=1
        float v1386 = v1378[v1383][v1384][v1385];	// L1645
        v1379[(v1383 + (v1380 * 8))][(v1384 + (v1381 * 7))][(v1385 + (v1382 * 7))] = v1386;	// L1646
      }
    }
  }
}

void forward_node108(
  float v1387[8][7][7],
  float v1388[8][7][7],
  float v1389[8][8],
  float v1390[8][7][7],
  float v1391[8][7][7],
  int v1392,
  int v1393,
  int v1394
) {	// L1652
  #pragma HLS inline
  for (int v1395 = 0; v1395 < 8; v1395 += 1) {	// L1653
    for (int v1396 = 0; v1396 < 8; v1396 += 2) {	// L1654
      for (int v1397 = 0; v1397 < 7; v1397 += 1) {	// L1655
        for (int v1398 = 0; v1398 < 7; v1398 += 1) {	// L1656
          #pragma HLS pipeline II=1
          float v1399 = v1388[v1396][v1397][v1398];	// L1657
          float v1400 = v1390[v1396][v1397][v1398];	// L1658
          float v1401 = v1391[v1396][v1397][v1398];	// L1659
          float v1402 = (v1395 == 0) ? v1400 : v1401;	// L1660
          float v1403 = ((v1395 + (v1393 * 8)) == 0 && v1392 == 0 && v1394 == 0) ? v1399 : v1402;	// L1661
          float v1404 = v1387[v1395][v1397][v1398];	// L1662
          float v1405 = v1389[v1396][v1395];	// L1663
          float v1406 = v1404 * v1405;	// L1664
          float v1407 = v1403 + v1406;	// L1665
          v1391[v1396][v1397][v1398] = v1407;	// L1666
          float v1408 = v1388[(v1396 + 1)][v1397][v1398];	// L1667
          float v1409 = v1390[(v1396 + 1)][v1397][v1398];	// L1668
          float v1410 = v1391[(v1396 + 1)][v1397][v1398];	// L1669
          float v1411 = (v1395 == 0) ? v1409 : v1410;	// L1670
          float v1412 = ((v1395 + (v1393 * 8)) == 0 && v1392 == 0 && v1394 == 0) ? v1408 : v1411;	// L1671
          float v1413 = v1389[(v1396 + 1)][v1395];	// L1672
          float v1414 = v1404 * v1413;	// L1673
          float v1415 = v1412 + v1414;	// L1674
          v1391[(v1396 + 1)][v1397][v1398] = v1415;	// L1675
        }
      }
    }
  }
}

void forward_node109(
  float v1416[128][128][3][3],
  float v1417[8][8],
  int v1418,
  int v1419,
  int v1420,
  int v1421
) {	// L1682
  #pragma HLS inline
  for (int v1422 = 0; v1422 < 8; v1422 += 1) {	// L1683
    for (int v1423 = 0; v1423 < 8; v1423 += 1) {	// L1684
      #pragma HLS pipeline II=1
      float v1424 = v1416[(v1422 + (v1420 * 8))][(v1423 + (v1421 * 8))][v1418][v1419];	// L1685
      v1417[v1422][v1423] = v1424;	// L1686
    }
  }
}

void forward_node110(
  float v1425[128][28][28],
  float v1426[8][7][7],
  int v1427,
  int v1428,
  int v1429,
  int v1430,
  int v1431
) {	// L1691
  #pragma HLS inline
  for (int v1432 = 0; v1432 < 8; v1432 += 1) {	// L1692
    for (int v1433 = 0; v1433 < 7; v1433 += 1) {	// L1693
      for (int v1434 = 0; v1434 < 7; v1434 += 1) {	// L1694
        #pragma HLS pipeline II=1
        float v1435 = v1425[(v1432 + (v1427 * 8))][(((v1433 + v1428) + (v1429 * 7)) - 1)][(((v1434 + v1430) + (v1431 * 7)) - 1)];	// L1695
        v1426[v1432][v1433][v1434] = v1435;	// L1696
      }
    }
  }
}

void forward_node111(
  float v1436[128][28][28],
  float v1437[8][7][7],
  int v1438,
  int v1439,
  int v1440
) {	// L1702
  #pragma HLS inline
  for (int v1441 = 0; v1441 < 8; v1441 += 1) {	// L1703
    for (int v1442 = 0; v1442 < 7; v1442 += 1) {	// L1704
      for (int v1443 = 0; v1443 < 7; v1443 += 1) {	// L1705
        #pragma HLS pipeline II=1
        float v1444 = v1436[(v1441 + (v1438 * 8))][(v1442 + (v1439 * 7))][(v1443 + (v1440 * 7))];	// L1706
        v1437[v1441][v1442][v1443] = v1444;	// L1707
      }
    }
  }
}

void forward_node112(
  float v1445[128][28][28],
  float v1446[8][7][7],
  int v1447,
  int v1448,
  int v1449
) {	// L1713
  #pragma HLS inline
  for (int v1450 = 0; v1450 < 8; v1450 += 1) {	// L1714
    for (int v1451 = 0; v1451 < 7; v1451 += 1) {	// L1715
      for (int v1452 = 0; v1452 < 7; v1452 += 1) {	// L1716
        #pragma HLS pipeline II=1
        float v1453 = v1445[(v1450 + (v1447 * 8))][(v1451 + (v1448 * 7))][(v1452 + (v1449 * 7))];	// L1717
        v1446[v1450][v1451][v1452] = v1453;	// L1718
      }
    }
  }
}

void forward_node106(
  float v1454[128][128][3][3],
  float v1455[128][28][28],
  float v1456[128][28][28],
  float v1457[128][28][28],
  float v1458[128][28][28]
) {	// L1724
  for (int v1459 = 0; v1459 < 36864; v1459 += 1) {	// L1725
    #pragma HLS dataflow
    int v1460 = (v1459 % 4);	// L1726
    int v1461 = ((v1459 / 4) % 4);	// L1727
    int v1462 = (((v1459 / 4) / 4) % 16);	// L1728
    int v1463 = ((((v1459 / 4) / 4) / 16) % 3);	// L1729
    int v1464 = (((((v1459 / 4) / 4) / 16) / 3) % 3);	// L1730
    int v1465 = (((((v1459 / 4) / 4) / 16) / 3) / 3);	// L1731
    float v1466[8][8];	// L1732
    #pragma HLS array_partition variable=v1466 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v1466 type=ram_2p impl=lutram

    float v1467[8][7][7];	// L1733
    #pragma HLS bind_storage variable=v1467 type=ram_t2p impl=bram

    float v1468[8][7][7];	// L1734
    #pragma HLS array_partition variable=v1468 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v1468 type=ram_2p impl=lutram

    float v1469[8][7][7];	// L1735
    #pragma HLS array_partition variable=v1469 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v1469 type=ram_2p impl=lutram

    forward_node112(v1455, v1469, v1462, v1461, v1460);	// L1736
    forward_node111(v1457, v1468, v1462, v1461, v1460);	// L1737
    forward_node110(v1456, v1467, v1465, v1464, v1461, v1463, v1460);	// L1738
    forward_node109(v1454, v1466, v1464, v1463, v1462, v1465);	// L1739
    float v1470[8][7][7];	// L1740
    #pragma HLS array_partition variable=v1470 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v1470 type=ram_2p impl=lutram

    forward_node108(v1467, v1469, v1466, v1468, v1470, v1464, v1465, v1463);	// L1741
    forward_node107(v1470, v1458, v1462, v1461, v1460);	// L1742
  }
}

void forward_node115(
  float v1471[7],
  float v1472[128][28][28],
  int v1473,
  int v1474,
  int v1475
) {	// L1746
  #pragma HLS inline
  for (int v1476 = 0; v1476 < 7; v1476 += 1) {	// L1747
    #pragma HLS pipeline II=1
    float v1477 = v1471[v1476];	// L1748
    v1472[v1473][v1474][(v1476 + (v1475 * 7))] = v1477;	// L1749
  }
}

void forward_node116(
  float v1478[7],
  float v1479[7]
) {	// L1753
  #pragma HLS inline
  for (int v1480 = 0; v1480 < 7; v1480 += 1) {	// L1755
    #pragma HLS pipeline II=1
    float v1481 = v1478[v1480];	// L1756
    bool v1482 = v1481 > (float)0.000000;	// L1757
    float v1483 = v1482 ? v1481 : (float)0.000000;	// L1758
    v1479[v1480] = v1483;	// L1759
  }
}

void forward_node117(
  float v1484[128][28][28],
  float v1485[7],
  int v1486,
  int v1487,
  int v1488
) {	// L1763
  #pragma HLS inline
  for (int v1489 = 0; v1489 < 7; v1489 += 1) {	// L1764
    #pragma HLS pipeline II=1
    float v1490 = v1484[v1486][v1487][(v1489 + (v1488 * 7))];	// L1765
    v1485[v1489] = v1490;	// L1766
  }
}

void forward_node118(
  float v1491[7],
  float v1492[128][28][28],
  int v1493,
  int v1494,
  int v1495
) {	// L1770
  #pragma HLS inline
  for (int v1496 = 0; v1496 < 7; v1496 += 1) {	// L1771
    #pragma HLS pipeline II=1
    float v1497 = v1491[v1496];	// L1772
    v1492[v1493][v1494][(v1496 + (v1495 * 7))] = v1497;	// L1773
  }
}

void forward_node119(
  float v1498[8][7],
  float v1499[8],
  float v1500[7],
  float v1501[7]
) {	// L1777
  #pragma HLS inline
  for (int v1502 = 0; v1502 < 8; v1502 += 4) {	// L1778
    for (int v1503 = 0; v1503 < 7; v1503 += 1) {	// L1779
      #pragma HLS pipeline II=1
      float v1504 = v1498[v1502][v1503];	// L1780
      float v1505 = v1499[v1502];	// L1781
      float v1506 = v1500[v1503];	// L1782
      float v1507 = v1501[v1503];	// L1783
      float v1508 = (v1502 == 0) ? v1506 : v1507;	// L1784
      float v1509 = v1504 * v1505;	// L1785
      float v1510 = v1508 + v1509;	// L1786
      int v1511 = (v1502 + 1);	// L1787
      float v1512 = v1498[(v1502 + 1)][v1503];	// L1788
      float v1513 = v1499[(v1502 + 1)];	// L1789
      float v1514 = (v1511 == 0) ? v1506 : v1510;	// L1790
      float v1515 = v1512 * v1513;	// L1791
      float v1516 = v1514 + v1515;	// L1792
      int v1517 = (v1502 + 2);	// L1793
      float v1518 = v1498[(v1502 + 2)][v1503];	// L1794
      float v1519 = v1499[(v1502 + 2)];	// L1795
      float v1520 = (v1517 == 0) ? v1506 : v1516;	// L1796
      float v1521 = v1518 * v1519;	// L1797
      float v1522 = v1520 + v1521;	// L1798
      int v1523 = (v1502 + 3);	// L1799
      float v1524 = v1498[(v1502 + 3)][v1503];	// L1800
      float v1525 = v1499[(v1502 + 3)];	// L1801
      float v1526 = (v1523 == 0) ? v1506 : v1522;	// L1802
      float v1527 = v1524 * v1525;	// L1803
      float v1528 = v1526 + v1527;	// L1804
      v1501[v1503] = v1528;	// L1805
    }
  }
}

void forward_node120(
  float v1529[128][28][28],
  float v1530[7],
  int v1531,
  int v1532,
  int v1533
) {	// L1810
  #pragma HLS inline
  for (int v1534 = 0; v1534 < 7; v1534 += 1) {	// L1811
    #pragma HLS pipeline II=1
    float v1535 = v1529[v1531][v1532][(v1534 + (v1533 * 7))];	// L1812
    v1530[v1534] = v1535;	// L1813
  }
}

void forward_node121(
  float v1536[128][64][3][3],
  float v1537[8],
  int v1538,
  int v1539,
  int v1540,
  int v1541
) {	// L1817
  #pragma HLS inline
  for (int v1542 = 0; v1542 < 8; v1542 += 1) {	// L1818
    #pragma HLS pipeline II=1
    float v1543 = v1536[v1538][(v1542 + (v1541 * 8))][v1539][v1540];	// L1819
    v1537[v1542] = v1543;	// L1820
  }
}

void forward_node122(
  float v1544[64][56][56],
  float v1545[8][7],
  int v1546,
  int v1547,
  int v1548,
  int v1549,
  int v1550
) {	// L1824
  #pragma HLS inline
  for (int v1551 = 0; v1551 < 8; v1551 += 4) {	// L1825
    for (int v1552 = 0; v1552 < 7; v1552 += 1) {	// L1826
      #pragma HLS pipeline II=1
      float v1553 = v1544[(v1551 + (v1546 * 8))][(((v1547 * 2) + v1548) - 1)][((((v1552 * 2) + v1549) + (v1550 * 14)) - 1)];	// L1827
      v1545[v1551][v1552] = v1553;	// L1828
      float v1554 = v1544[((v1551 + (v1546 * 8)) + 1)][(((v1547 * 2) + v1548) - 1)][((((v1552 * 2) + v1549) + (v1550 * 14)) - 1)];	// L1829
      v1545[(v1551 + 1)][v1552] = v1554;	// L1830
      float v1555 = v1544[((v1551 + (v1546 * 8)) + 2)][(((v1547 * 2) + v1548) - 1)][((((v1552 * 2) + v1549) + (v1550 * 14)) - 1)];	// L1831
      v1545[(v1551 + 2)][v1552] = v1555;	// L1832
      float v1556 = v1544[((v1551 + (v1546 * 8)) + 3)][(((v1547 * 2) + v1548) - 1)][((((v1552 * 2) + v1549) + (v1550 * 14)) - 1)];	// L1833
      v1545[(v1551 + 3)][v1552] = v1556;	// L1834
    }
  }
}

void forward_node114(
  float v1557[64][56][56],
  float v1558[128][64][3][3],
  float v1559[28],
  float v1560[128][28][28],
  float v1561[128][28][28],
  float v1562[128][28][28],
  float v1563[128][28][28],
  int v1564,
  int v1565
) {	// L1839
  for (int v1566 = 0; v1566 < 3; v1566 += 1) {	// L1840
    for (int v1567 = 0; v1567 < 8; v1567 += 1) {	// L1841
      for (int v1568 = 0; v1568 < 4; v1568 += 1) {	// L1842
        for (int v1569 = 0; v1569 < 3; v1569 += 1) {	// L1843
          float v1570[7];	// L1844
          #pragma HLS bind_storage variable=v1570 type=ram_t2p impl=bram

          float v1571[8];	// L1845
          #pragma HLS array_partition variable=v1571 cyclic factor=4 dim=1
          #pragma HLS bind_storage variable=v1571 type=ram_2p impl=lutram

          float v1572[8][7];	// L1846
          #pragma HLS array_partition variable=v1572 cyclic factor=4 dim=1
          #pragma HLS bind_storage variable=v1572 type=ram_2p impl=lutram

          forward_node122(v1557, v1572, v1567, v1565, v1566, v1569, v1568);	// L1847
          forward_node121(v1558, v1571, v1564, v1566, v1569, v1567);	// L1848
          forward_node120(v1560, v1570, v1564, v1565, v1568);	// L1849
          float v1573[7];	// L1850
          #pragma HLS bind_storage variable=v1573 type=ram_t2p impl=bram

          forward_node119(v1572, v1571, v1570, v1573);	// L1851
          forward_node118(v1573, v1562, v1564, v1565, v1568);	// L1852
        }
      }
    }
  }
  for (int v1574 = 0; v1574 < 4; v1574 += 1) {	// L1857
    #pragma HLS dataflow
    float v1575[7];	// L1858
    #pragma HLS bind_storage variable=v1575 type=ram_t2p impl=bram

    forward_node117(v1561, v1575, v1564, v1565, v1574);	// L1859
    float v1576[7];	// L1860
    #pragma HLS bind_storage variable=v1576 type=ram_t2p impl=bram

    forward_node116(v1575, v1576);	// L1861
    forward_node115(v1576, v1563, v1564, v1565, v1574);	// L1862
  }
  for (int v1577 = 0; v1577 < 28; v1577 += 2) {	// L1864
    #pragma HLS pipeline II=1
    float v1578 = v1559[v1577];	// L1865
    v1562[v1564][v1565][v1577] = v1578;	// L1866
    float v1579 = v1559[(v1577 + 1)];	// L1867
    v1562[v1564][v1565][(v1577 + 1)] = v1579;	// L1868
  }
}

void forward_node123(
  float v1580[28],
  float v1581[28]
) {	// L1872
  #pragma HLS inline
  for (int v1582 = 0; v1582 < 4; v1582 += 1) {	// L1873
    for (int v1583 = 0; v1583 < 7; v1583 += 1) {	// L1874
      #pragma HLS pipeline II=1
      float v1584 = v1580[(v1583 + (v1582 * 7))];	// L1875
      v1581[(v1583 + (v1582 * 7))] = v1584;	// L1876
    }
  }
}

void forward_node124(
  float v1585[128][28][28],
  float v1586[28],
  int v1587,
  int v1588
) {	// L1881
  #pragma HLS inline
  for (int v1589 = 0; v1589 < 28; v1589 += 1) {	// L1882
    #pragma HLS pipeline II=1
    float v1590 = v1585[v1587][v1588][v1589];	// L1883
    v1586[v1589] = v1590;	// L1884
  }
}

void forward_node113(
  float v1591[128][28][28],
  float v1592[64][56][56],
  float v1593[128][64][3][3],
  float v1594[128][28][28],
  float v1595[128][28][28],
  float v1596[128][28][28],
  float v1597[128][28][28]
) {	// L1888
  for (int v1598 = 0; v1598 < 3584; v1598 += 1) {	// L1889
    #pragma HLS dataflow
    int v1599 = (v1598 % 28);	// L1890
    int v1600 = (v1598 / 28);	// L1891
    float v1601[28];	// L1892
    #pragma HLS array_partition variable=v1601 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v1601 type=ram_2p impl=lutram

    float v1602[28];	// L1893
    #pragma HLS bind_storage variable=v1602 type=ram_t2p impl=bram

    forward_node124(v1591, v1602, v1600, v1599);	// L1894
    forward_node123(v1602, v1601);	// L1895
    forward_node114(v1592, v1593, v1601, v1594, v1595, v1596, v1597, v1600, v1599);	// L1896
  }
}

void forward_node126(
  float v1603[8][8][8],
  float v1604[64][56][56],
  int v1605,
  int v1606,
  int v1607
) {	// L1900
  #pragma HLS inline
  for (int v1608 = 0; v1608 < 8; v1608 += 1) {	// L1901
    for (int v1609 = 0; v1609 < 8; v1609 += 2) {	// L1902
      for (int v1610 = 0; v1610 < 8; v1610 += 2) {	// L1903
        #pragma HLS pipeline II=1
        float v1611 = v1603[v1608][v1609][v1610];	// L1904
        v1604[(v1608 + (v1605 * 8))][(v1609 + (v1606 * 8))][(v1610 + (v1607 * 8))] = v1611;	// L1905
        float v1612 = v1603[v1608][v1609][(v1610 + 1)];	// L1906
        v1604[(v1608 + (v1605 * 8))][(v1609 + (v1606 * 8))][((v1610 + (v1607 * 8)) + 1)] = v1612;	// L1907
        float v1613 = v1603[v1608][(v1609 + 1)][v1610];	// L1908
        v1604[(v1608 + (v1605 * 8))][((v1609 + (v1606 * 8)) + 1)][(v1610 + (v1607 * 8))] = v1613;	// L1909
        float v1614 = v1603[v1608][(v1609 + 1)][(v1610 + 1)];	// L1910
        v1604[(v1608 + (v1605 * 8))][((v1609 + (v1606 * 8)) + 1)][((v1610 + (v1607 * 8)) + 1)] = v1614;	// L1911
      }
    }
  }
}

void forward_node127(
  float v1615[8][8][8],
  float v1616[64][56][56],
  int v1617,
  int v1618,
  int v1619
) {	// L1917
  #pragma HLS inline
  for (int v1620 = 0; v1620 < 8; v1620 += 1) {	// L1918
    for (int v1621 = 0; v1621 < 8; v1621 += 1) {	// L1919
      for (int v1622 = 0; v1622 < 8; v1622 += 1) {	// L1920
        #pragma HLS pipeline II=1
        float v1623 = v1615[v1620][v1621][v1622];	// L1921
        v1616[(v1620 + (v1617 * 8))][(v1621 + (v1618 * 8))][(v1622 + (v1619 * 8))] = v1623;	// L1922
      }
    }
  }
}

void forward_node128(
  float v1624[8][8][8],
  float v1625[8][8][8],
  float v1626[8][8],
  float v1627[8][8][8],
  float v1628[8][8][8],
  float v1629[8][8][8],
  int v1630,
  int v1631,
  int v1632
) {	// L1928
  #pragma HLS inline
  for (int v1633 = 0; v1633 < 8; v1633 += 1) {	// L1930
    for (int v1634 = 0; v1634 < 8; v1634 += 1) {	// L1931
      for (int v1635 = 0; v1635 < 8; v1635 += 1) {	// L1932
        for (int v1636 = 0; v1636 < 8; v1636 += 2) {	// L1933
          #pragma HLS pipeline II=1
          float v1637 = v1625[v1633][v1635][v1636];	// L1934
          float v1638 = v1626[v1634][v1633];	// L1935
          float v1639 = v1627[v1634][v1635][v1636];	// L1936
          float v1640 = v1629[v1634][v1635][v1636];	// L1937
          float v1641 = (v1633 == 0) ? v1639 : v1640;	// L1938
          float v1642 = v1637 * v1638;	// L1939
          float v1643 = v1641 + v1642;	// L1940
          v1629[v1634][v1635][v1636] = v1643;	// L1941
          float v1644 = v1624[v1634][v1635][v1636];	// L1942
          float v1645 = v1643 + v1644;	// L1943
          bool v1646 = v1645 > (float)0.000000;	// L1944
          float v1647 = v1646 ? v1645 : (float)0.000000;	// L1945
          if ((((-v1633) + (v1632 * -8)) + 63) == 0 && ((-v1630) + 2) == 0 && ((-v1631) + 2) == 0) {	// L1946
            v1628[v1634][v1635][v1636] = v1647;	// L1947
          }
          float v1648 = v1625[v1633][v1635][(v1636 + 1)];	// L1949
          float v1649 = v1627[v1634][v1635][(v1636 + 1)];	// L1950
          float v1650 = v1629[v1634][v1635][(v1636 + 1)];	// L1951
          float v1651 = (v1633 == 0) ? v1649 : v1650;	// L1952
          float v1652 = v1648 * v1638;	// L1953
          float v1653 = v1651 + v1652;	// L1954
          v1629[v1634][v1635][(v1636 + 1)] = v1653;	// L1955
          float v1654 = v1624[v1634][v1635][(v1636 + 1)];	// L1956
          float v1655 = v1653 + v1654;	// L1957
          bool v1656 = v1655 > (float)0.000000;	// L1958
          float v1657 = v1656 ? v1655 : (float)0.000000;	// L1959
          if ((((-v1633) + (v1632 * -8)) + 63) == 0 && ((-v1630) + 2) == 0 && ((-v1631) + 2) == 0) {	// L1960
            v1628[v1634][v1635][(v1636 + 1)] = v1657;	// L1961
          }
        }
      }
    }
  }
}

void forward_node129(
  float v1658[64][56][56],
  float v1659[8][8][8],
  int v1660,
  int v1661,
  int v1662
) {	// L1969
  #pragma HLS inline
  for (int v1663 = 0; v1663 < 8; v1663 += 1) {	// L1970
    for (int v1664 = 0; v1664 < 8; v1664 += 1) {	// L1971
      for (int v1665 = 0; v1665 < 8; v1665 += 1) {	// L1972
        #pragma HLS pipeline II=1
        float v1666 = v1658[(v1663 + (v1660 * 8))][(v1664 + (v1661 * 8))][(v1665 + (v1662 * 8))];	// L1973
        v1659[v1663][v1664][v1665] = v1666;	// L1974
      }
    }
  }
}

void forward_node130(
  float v1667[64][56][56],
  float v1668[8][8][8],
  int v1669,
  int v1670,
  int v1671
) {	// L1980
  #pragma HLS inline
  for (int v1672 = 0; v1672 < 8; v1672 += 1) {	// L1981
    for (int v1673 = 0; v1673 < 8; v1673 += 1) {	// L1982
      for (int v1674 = 0; v1674 < 8; v1674 += 1) {	// L1983
        #pragma HLS pipeline II=1
        float v1675 = v1667[(v1672 + (v1669 * 8))][(v1673 + (v1670 * 8))][(v1674 + (v1671 * 8))];	// L1984
        v1668[v1672][v1673][v1674] = v1675;	// L1985
      }
    }
  }
}

void forward_node131(
  float v1676[64][64][3][3],
  float v1677[8][8],
  int v1678,
  int v1679,
  int v1680,
  int v1681
) {	// L1991
  #pragma HLS inline
  for (int v1682 = 0; v1682 < 8; v1682 += 1) {	// L1992
    for (int v1683 = 0; v1683 < 8; v1683 += 1) {	// L1993
      #pragma HLS pipeline II=1
      float v1684 = v1676[(v1682 + (v1680 * 8))][(v1683 + (v1681 * 8))][v1678][v1679];	// L1994
      v1677[v1682][v1683] = v1684;	// L1995
    }
  }
}

void forward_node132(
  float v1685[64][56][56],
  float v1686[8][8][8],
  int v1687,
  int v1688,
  int v1689,
  int v1690,
  int v1691
) {	// L2000
  #pragma HLS inline
  for (int v1692 = 0; v1692 < 8; v1692 += 1) {	// L2001
    for (int v1693 = 0; v1693 < 8; v1693 += 1) {	// L2002
      for (int v1694 = 0; v1694 < 8; v1694 += 1) {	// L2003
        #pragma HLS pipeline II=1
        float v1695 = v1685[(v1692 + (v1687 * 8))][(((v1693 + v1688) + (v1689 * 8)) - 1)][(((v1694 + v1690) + (v1691 * 8)) - 1)];	// L2004
        v1686[v1692][v1693][v1694] = v1695;	// L2005
      }
    }
  }
}

void forward_node125(
  float v1696[64][64][3][3],
  float v1697[64][56][56],
  float v1698[64][56][56],
  float v1699[64][56][56],
  float v1700[64][56][56],
  float v1701[64][56][56]
) {	// L2011
  for (int v1702 = 0; v1702 < 28224; v1702 += 1) {	// L2012
    #pragma HLS dataflow
    int v1703 = (v1702 % 7);	// L2013
    int v1704 = ((v1702 / 7) % 7);	// L2014
    int v1705 = (((v1702 / 7) / 7) % 8);	// L2015
    int v1706 = ((((v1702 / 7) / 7) / 8) % 3);	// L2016
    int v1707 = (((((v1702 / 7) / 7) / 8) / 3) % 3);	// L2017
    int v1708 = (((((v1702 / 7) / 7) / 8) / 3) / 3);	// L2018
    float v1709[8][8][8];	// L2019
    #pragma HLS array_partition variable=v1709 cyclic factor=2 dim=2
    #pragma HLS array_partition variable=v1709 cyclic factor=2 dim=3
    #pragma HLS bind_storage variable=v1709 type=ram_2p impl=lutram

    float v1710[8][8][8];	// L2020
    #pragma HLS array_partition variable=v1710 cyclic factor=2 dim=3
    #pragma HLS bind_storage variable=v1710 type=ram_2p impl=lutram

    float v1711[8][8][8];	// L2021
    #pragma HLS array_partition variable=v1711 cyclic factor=2 dim=3
    #pragma HLS bind_storage variable=v1711 type=ram_2p impl=lutram

    float v1712[8][8];	// L2022
    #pragma HLS bind_storage variable=v1712 type=ram_t2p impl=bram

    float v1713[8][8][8];	// L2023
    #pragma HLS array_partition variable=v1713 cyclic factor=2 dim=3
    #pragma HLS bind_storage variable=v1713 type=ram_2p impl=lutram

    forward_node132(v1698, v1713, v1708, v1707, v1704, v1706, v1703);	// L2024
    forward_node131(v1696, v1712, v1707, v1706, v1705, v1708);	// L2025
    forward_node130(v1699, v1711, v1705, v1704, v1703);	// L2026
    forward_node129(v1697, v1710, v1705, v1704, v1703);	// L2027
    float v1714[8][8][8];	// L2028
    #pragma HLS array_partition variable=v1714 cyclic factor=2 dim=3
    #pragma HLS bind_storage variable=v1714 type=ram_2p impl=lutram

    forward_node128(v1710, v1713, v1712, v1711, v1709, v1714, v1707, v1706, v1708);	// L2029
    forward_node127(v1714, v1701, v1705, v1704, v1703);	// L2030
    forward_node126(v1709, v1700, v1705, v1704, v1703);	// L2031
  }
}

void forward_node134(
  float v1715[8][8][8],
  float v1716[64][56][56],
  int v1717,
  int v1718,
  int v1719
) {	// L2035
  #pragma HLS inline
  for (int v1720 = 0; v1720 < 8; v1720 += 1) {	// L2036
    for (int v1721 = 0; v1721 < 8; v1721 += 1) {	// L2037
      for (int v1722 = 0; v1722 < 8; v1722 += 1) {	// L2038
        #pragma HLS pipeline II=1
        float v1723 = v1715[v1720][v1721][v1722];	// L2039
        v1716[(v1720 + (v1717 * 8))][(v1721 + (v1718 * 8))][(v1722 + (v1719 * 8))] = v1723;	// L2040
      }
    }
  }
}

void forward_node135(
  float v1724[8][8],
  float v1725[8][8][8],
  float v1726[8][8][8],
  float v1727[8][8][8],
  float v1728[8][8][8],
  int v1729,
  int v1730,
  int v1731
) {	// L2046
  #pragma HLS inline
  for (int v1732 = 0; v1732 < 8; v1732 += 1) {	// L2048
    for (int v1733 = 0; v1733 < 8; v1733 += 1) {	// L2049
      for (int v1734 = 0; v1734 < 8; v1734 += 1) {	// L2050
        for (int v1735 = 0; v1735 < 8; v1735 += 2) {	// L2051
          #pragma HLS pipeline II=1
          float v1736 = v1726[v1733][v1734][v1735];	// L2052
          float v1737 = v1727[v1733][v1734][v1735];	// L2053
          float v1738 = v1728[v1733][v1734][v1735];	// L2054
          float v1739 = (v1732 == 0) ? v1737 : v1738;	// L2055
          float v1740 = ((v1732 + (v1729 * 8)) == 0 && v1730 == 0 && v1731 == 0) ? v1736 : v1739;	// L2056
          float v1741 = v1725[v1732][v1734][v1735];	// L2057
          float v1742 = v1724[v1733][v1732];	// L2058
          float v1743 = v1741 * v1742;	// L2059
          float v1744 = v1740 + v1743;	// L2060
          bool v1745 = v1744 > (float)0.000000;	// L2061
          float v1746 = v1745 ? v1744 : (float)0.000000;	// L2062
          float v1747 = ((((-v1732) + (v1729 * -8)) + 63) == 0 && ((-v1730) + 2) == 0 && ((-v1731) + 2) == 0) ? v1746 : v1744;	// L2063
          v1728[v1733][v1734][v1735] = v1747;	// L2064
          float v1748 = v1726[v1733][v1734][(v1735 + 1)];	// L2065
          float v1749 = v1727[v1733][v1734][(v1735 + 1)];	// L2066
          float v1750 = v1728[v1733][v1734][(v1735 + 1)];	// L2067
          float v1751 = (v1732 == 0) ? v1749 : v1750;	// L2068
          float v1752 = ((v1732 + (v1729 * 8)) == 0 && v1730 == 0 && v1731 == 0) ? v1748 : v1751;	// L2069
          float v1753 = v1725[v1732][v1734][(v1735 + 1)];	// L2070
          float v1754 = v1753 * v1742;	// L2071
          float v1755 = v1752 + v1754;	// L2072
          bool v1756 = v1755 > (float)0.000000;	// L2073
          float v1757 = v1756 ? v1755 : (float)0.000000;	// L2074
          float v1758 = ((((-v1732) + (v1729 * -8)) + 63) == 0 && ((-v1730) + 2) == 0 && ((-v1731) + 2) == 0) ? v1757 : v1755;	// L2075
          v1728[v1733][v1734][(v1735 + 1)] = v1758;	// L2076
        }
      }
    }
  }
}

void forward_node136(
  float v1759[64][64][3][3],
  float v1760[8][8],
  int v1761,
  int v1762,
  int v1763,
  int v1764
) {	// L2083
  #pragma HLS inline
  for (int v1765 = 0; v1765 < 8; v1765 += 1) {	// L2084
    for (int v1766 = 0; v1766 < 8; v1766 += 1) {	// L2085
      #pragma HLS pipeline II=1
      float v1767 = v1759[(v1765 + (v1763 * 8))][(v1766 + (v1764 * 8))][v1761][v1762];	// L2086
      v1760[v1765][v1766] = v1767;	// L2087
    }
  }
}

void forward_node137(
  float v1768[64][56][56],
  float v1769[8][8][8],
  int v1770,
  int v1771,
  int v1772,
  int v1773,
  int v1774
) {	// L2092
  #pragma HLS inline
  for (int v1775 = 0; v1775 < 8; v1775 += 1) {	// L2093
    for (int v1776 = 0; v1776 < 8; v1776 += 1) {	// L2094
      for (int v1777 = 0; v1777 < 8; v1777 += 1) {	// L2095
        #pragma HLS pipeline II=1
        float v1778 = v1768[(v1775 + (v1770 * 8))][(((v1776 + v1771) + (v1772 * 8)) - 1)][(((v1777 + v1773) + (v1774 * 8)) - 1)];	// L2096
        v1769[v1775][v1776][v1777] = v1778;	// L2097
      }
    }
  }
}

void forward_node138(
  float v1779[64][56][56],
  float v1780[8][8][8],
  int v1781,
  int v1782,
  int v1783
) {	// L2103
  #pragma HLS inline
  for (int v1784 = 0; v1784 < 8; v1784 += 1) {	// L2104
    for (int v1785 = 0; v1785 < 8; v1785 += 1) {	// L2105
      for (int v1786 = 0; v1786 < 8; v1786 += 1) {	// L2106
        #pragma HLS pipeline II=1
        float v1787 = v1779[(v1784 + (v1781 * 8))][(v1785 + (v1782 * 8))][(v1786 + (v1783 * 8))];	// L2107
        v1780[v1784][v1785][v1786] = v1787;	// L2108
      }
    }
  }
}

void forward_node139(
  float v1788[64][56][56],
  float v1789[8][8][8],
  int v1790,
  int v1791,
  int v1792
) {	// L2114
  #pragma HLS inline
  for (int v1793 = 0; v1793 < 8; v1793 += 1) {	// L2115
    for (int v1794 = 0; v1794 < 8; v1794 += 1) {	// L2116
      for (int v1795 = 0; v1795 < 8; v1795 += 1) {	// L2117
        #pragma HLS pipeline II=1
        float v1796 = v1788[(v1793 + (v1790 * 8))][(v1794 + (v1791 * 8))][(v1795 + (v1792 * 8))];	// L2118
        v1789[v1793][v1794][v1795] = v1796;	// L2119
      }
    }
  }
}

void forward_node133(
  float v1797[64][64][3][3],
  float v1798[64][56][56],
  float v1799[64][56][56],
  float v1800[64][56][56],
  float v1801[64][56][56]
) {	// L2125
  for (int v1802 = 0; v1802 < 28224; v1802 += 1) {	// L2126
    #pragma HLS dataflow
    int v1803 = (v1802 % 7);	// L2127
    int v1804 = ((v1802 / 7) % 7);	// L2128
    int v1805 = (((v1802 / 7) / 7) % 8);	// L2129
    int v1806 = ((((v1802 / 7) / 7) / 8) % 3);	// L2130
    int v1807 = (((((v1802 / 7) / 7) / 8) / 3) % 3);	// L2131
    int v1808 = (((((v1802 / 7) / 7) / 8) / 3) / 3);	// L2132
    float v1809[8][8];	// L2133
    #pragma HLS bind_storage variable=v1809 type=ram_t2p impl=bram

    float v1810[8][8][8];	// L2134
    #pragma HLS array_partition variable=v1810 cyclic factor=2 dim=3
    #pragma HLS bind_storage variable=v1810 type=ram_2p impl=lutram

    float v1811[8][8][8];	// L2135
    #pragma HLS array_partition variable=v1811 cyclic factor=2 dim=3
    #pragma HLS bind_storage variable=v1811 type=ram_2p impl=lutram

    float v1812[8][8][8];	// L2136
    #pragma HLS array_partition variable=v1812 cyclic factor=2 dim=3
    #pragma HLS bind_storage variable=v1812 type=ram_2p impl=lutram

    forward_node139(v1799, v1812, v1805, v1804, v1803);	// L2137
    forward_node138(v1800, v1811, v1805, v1804, v1803);	// L2138
    forward_node137(v1798, v1810, v1808, v1807, v1804, v1806, v1803);	// L2139
    forward_node136(v1797, v1809, v1807, v1806, v1805, v1808);	// L2140
    float v1813[8][8][8];	// L2141
    #pragma HLS array_partition variable=v1813 cyclic factor=2 dim=3
    #pragma HLS bind_storage variable=v1813 type=ram_2p impl=lutram

    forward_node135(v1809, v1810, v1812, v1811, v1813, v1808, v1807, v1806);	// L2142
    forward_node134(v1813, v1801, v1805, v1804, v1803);	// L2143
  }
}

void forward_node141(
  float v1814[8][8][8],
  float v1815[64][56][56],
  int v1816,
  int v1817,
  int v1818
) {	// L2147
  #pragma HLS inline
  for (int v1819 = 0; v1819 < 8; v1819 += 1) {	// L2148
    for (int v1820 = 0; v1820 < 8; v1820 += 1) {	// L2149
      for (int v1821 = 0; v1821 < 8; v1821 += 1) {	// L2150
        #pragma HLS pipeline II=1
        float v1822 = v1814[v1819][v1820][v1821];	// L2151
        v1815[(v1819 + (v1816 * 8))][(v1820 + (v1817 * 8))][(v1821 + (v1818 * 8))] = v1822;	// L2152
      }
    }
  }
}

void forward_node142(
  float v1823[8][8][8],
  float v1824[64][56][56],
  int v1825,
  int v1826,
  int v1827
) {	// L2158
  #pragma HLS inline
  for (int v1828 = 0; v1828 < 8; v1828 += 1) {	// L2159
    for (int v1829 = 0; v1829 < 8; v1829 += 1) {	// L2160
      for (int v1830 = 0; v1830 < 8; v1830 += 1) {	// L2161
        #pragma HLS pipeline II=1
        float v1831 = v1823[v1828][v1829][v1830];	// L2162
        v1824[(v1828 + (v1825 * 8))][(v1829 + (v1826 * 8))][(v1830 + (v1827 * 8))] = v1831;	// L2163
      }
    }
  }
}

void forward_node143(
  float v1832[8][8][8],
  float v1833[8][8][8],
  float v1834[8][8],
  float v1835[8][8][8],
  float v1836[8][8][8],
  float v1837[8][8][8],
  float v1838[8][8][8],
  int v1839,
  int v1840,
  int v1841
) {	// L2169
  #pragma HLS inline
  for (int v1842 = 0; v1842 < 8; v1842 += 1) {	// L2171
    for (int v1843 = 0; v1843 < 8; v1843 += 1) {	// L2172
      for (int v1844 = 0; v1844 < 8; v1844 += 1) {	// L2173
        for (int v1845 = 0; v1845 < 8; v1845 += 2) {	// L2174
          #pragma HLS pipeline II=1
          float v1846 = v1833[v1843][v1844][v1845];	// L2175
          float v1847 = v1836[v1843][v1844][v1845];	// L2176
          float v1848 = v1838[v1843][v1844][v1845];	// L2177
          float v1849 = (v1842 == 0) ? v1847 : v1848;	// L2178
          float v1850 = ((v1842 + (v1841 * 8)) == 0 && v1840 == 0 && v1839 == 0) ? v1846 : v1849;	// L2179
          float v1851 = v1835[v1842][v1844][v1845];	// L2180
          float v1852 = v1834[v1843][v1842];	// L2181
          float v1853 = v1851 * v1852;	// L2182
          float v1854 = v1850 + v1853;	// L2183
          v1838[v1843][v1844][v1845] = v1854;	// L2184
          float v1855 = v1832[v1843][v1844][v1845];	// L2185
          float v1856 = v1854 + v1855;	// L2186
          bool v1857 = v1856 > (float)0.000000;	// L2187
          float v1858 = v1857 ? v1856 : (float)0.000000;	// L2188
          if ((((-v1842) + (v1841 * -8)) + 63) == 0 && ((-v1840) + 2) == 0 && ((-v1839) + 2) == 0) {	// L2189
            v1837[v1843][v1844][v1845] = v1858;	// L2190
          }
          float v1859 = v1833[v1843][v1844][(v1845 + 1)];	// L2192
          float v1860 = v1836[v1843][v1844][(v1845 + 1)];	// L2193
          float v1861 = v1838[v1843][v1844][(v1845 + 1)];	// L2194
          float v1862 = (v1842 == 0) ? v1860 : v1861;	// L2195
          float v1863 = ((v1842 + (v1841 * 8)) == 0 && v1840 == 0 && v1839 == 0) ? v1859 : v1862;	// L2196
          float v1864 = v1835[v1842][v1844][(v1845 + 1)];	// L2197
          float v1865 = v1864 * v1852;	// L2198
          float v1866 = v1863 + v1865;	// L2199
          v1838[v1843][v1844][(v1845 + 1)] = v1866;	// L2200
          float v1867 = v1832[v1843][v1844][(v1845 + 1)];	// L2201
          float v1868 = v1866 + v1867;	// L2202
          bool v1869 = v1868 > (float)0.000000;	// L2203
          float v1870 = v1869 ? v1868 : (float)0.000000;	// L2204
          if ((((-v1842) + (v1841 * -8)) + 63) == 0 && ((-v1840) + 2) == 0 && ((-v1839) + 2) == 0) {	// L2205
            v1837[v1843][v1844][(v1845 + 1)] = v1870;	// L2206
          }
        }
      }
    }
  }
}

void forward_node144(
  float v1871[64][56][56],
  float v1872[8][8][8],
  int v1873,
  int v1874,
  int v1875
) {	// L2214
  #pragma HLS inline
  for (int v1876 = 0; v1876 < 8; v1876 += 1) {	// L2215
    for (int v1877 = 0; v1877 < 8; v1877 += 1) {	// L2216
      for (int v1878 = 0; v1878 < 8; v1878 += 2) {	// L2217
        #pragma HLS pipeline II=1
        float v1879 = v1871[(v1876 + (v1873 * 8))][(v1877 + (v1874 * 8))][(v1878 + (v1875 * 8))];	// L2218
        v1872[v1876][v1877][v1878] = v1879;	// L2219
        float v1880 = v1871[(v1876 + (v1873 * 8))][(v1877 + (v1874 * 8))][((v1878 + (v1875 * 8)) + 1)];	// L2220
        v1872[v1876][v1877][(v1878 + 1)] = v1880;	// L2221
      }
    }
  }
}

void forward_node145(
  float v1881[64][64][3][3],
  float v1882[8][8],
  int v1883,
  int v1884,
  int v1885,
  int v1886
) {	// L2227
  #pragma HLS inline
  for (int v1887 = 0; v1887 < 8; v1887 += 1) {	// L2228
    for (int v1888 = 0; v1888 < 8; v1888 += 1) {	// L2229
      #pragma HLS pipeline II=1
      float v1889 = v1881[(v1887 + (v1885 * 8))][(v1888 + (v1886 * 8))][v1883][v1884];	// L2230
      v1882[v1887][v1888] = v1889;	// L2231
    }
  }
}

void forward_node146(
  float v1890[64][56][56],
  float v1891[8][8][8],
  int v1892,
  int v1893,
  int v1894,
  int v1895,
  int v1896
) {	// L2236
  #pragma HLS inline
  for (int v1897 = 0; v1897 < 8; v1897 += 1) {	// L2237
    for (int v1898 = 0; v1898 < 8; v1898 += 1) {	// L2238
      for (int v1899 = 0; v1899 < 8; v1899 += 1) {	// L2239
        #pragma HLS pipeline II=1
        float v1900 = v1890[(v1897 + (v1892 * 8))][(((v1898 + v1893) + (v1894 * 8)) - 1)][(((v1899 + v1895) + (v1896 * 8)) - 1)];	// L2240
        v1891[v1897][v1898][v1899] = v1900;	// L2241
      }
    }
  }
}

void forward_node147(
  float v1901[64][56][56],
  float v1902[8][8][8],
  int v1903,
  int v1904,
  int v1905
) {	// L2247
  #pragma HLS inline
  for (int v1906 = 0; v1906 < 8; v1906 += 1) {	// L2248
    for (int v1907 = 0; v1907 < 8; v1907 += 1) {	// L2249
      for (int v1908 = 0; v1908 < 8; v1908 += 1) {	// L2250
        #pragma HLS pipeline II=1
        float v1909 = v1901[(v1906 + (v1903 * 8))][(v1907 + (v1904 * 8))][(v1908 + (v1905 * 8))];	// L2251
        v1902[v1906][v1907][v1908] = v1909;	// L2252
      }
    }
  }
}

void forward_node148(
  float v1910[64][56][56],
  float v1911[8][8][8],
  int v1912,
  int v1913,
  int v1914
) {	// L2258
  #pragma HLS inline
  for (int v1915 = 0; v1915 < 8; v1915 += 1) {	// L2259
    for (int v1916 = 0; v1916 < 8; v1916 += 1) {	// L2260
      for (int v1917 = 0; v1917 < 8; v1917 += 1) {	// L2261
        #pragma HLS pipeline II=1
        float v1918 = v1910[(v1915 + (v1912 * 8))][(v1916 + (v1913 * 8))][(v1917 + (v1914 * 8))];	// L2262
        v1911[v1915][v1916][v1917] = v1918;	// L2263
      }
    }
  }
}

void forward_node140(
  float v1919[64][64][3][3],
  float v1920[64][56][56],
  float v1921[64][56][56],
  float v1922[64][56][56],
  float v1923[64][56][56],
  float v1924[64][56][56],
  float v1925[64][56][56]
) {	// L2269
  for (int v1926 = 0; v1926 < 28224; v1926 += 1) {	// L2270
    #pragma HLS dataflow
    int v1927 = (v1926 % 7);	// L2271
    int v1928 = ((v1926 / 7) % 7);	// L2272
    int v1929 = (((v1926 / 7) / 7) % 8);	// L2273
    int v1930 = ((((v1926 / 7) / 7) / 8) % 3);	// L2274
    int v1931 = (((((v1926 / 7) / 7) / 8) / 3) % 3);	// L2275
    int v1932 = (((((v1926 / 7) / 7) / 8) / 3) / 3);	// L2276
    float v1933[8][8][8];	// L2277
    #pragma HLS array_partition variable=v1933 cyclic factor=2 dim=3
    #pragma HLS bind_storage variable=v1933 type=ram_2p impl=lutram

    float v1934[8][8][8];	// L2278
    #pragma HLS array_partition variable=v1934 cyclic factor=2 dim=3
    #pragma HLS bind_storage variable=v1934 type=ram_2p impl=lutram

    float v1935[8][8];	// L2279
    #pragma HLS bind_storage variable=v1935 type=ram_t2p impl=bram

    float v1936[8][8][8];	// L2280
    #pragma HLS array_partition variable=v1936 cyclic factor=2 dim=3
    #pragma HLS bind_storage variable=v1936 type=ram_2p impl=lutram

    float v1937[8][8][8];	// L2281
    #pragma HLS array_partition variable=v1937 cyclic factor=2 dim=3
    #pragma HLS bind_storage variable=v1937 type=ram_2p impl=lutram

    float v1938[8][8][8];	// L2282
    #pragma HLS array_partition variable=v1938 cyclic factor=2 dim=3
    #pragma HLS bind_storage variable=v1938 type=ram_2p impl=lutram

    forward_node148(v1922, v1938, v1929, v1928, v1927);	// L2283
    forward_node147(v1923, v1937, v1929, v1928, v1927);	// L2284
    forward_node146(v1920, v1936, v1932, v1931, v1928, v1930, v1927);	// L2285
    forward_node145(v1919, v1935, v1931, v1930, v1929, v1932);	// L2286
    forward_node144(v1921, v1934, v1929, v1928, v1927);	// L2287
    float v1939[8][8][8];	// L2288
    #pragma HLS array_partition variable=v1939 cyclic factor=2 dim=3
    #pragma HLS bind_storage variable=v1939 type=ram_2p impl=lutram

    forward_node143(v1934, v1938, v1935, v1936, v1937, v1933, v1939, v1930, v1931, v1932);	// L2289
    forward_node142(v1939, v1925, v1929, v1928, v1927);	// L2290
    forward_node141(v1933, v1924, v1929, v1928, v1927);	// L2291
  }
}

void forward_node150(
  float v1940[8][8][8],
  float v1941[64][56][56],
  int v1942,
  int v1943,
  int v1944
) {	// L2295
  #pragma HLS inline
  for (int v1945 = 0; v1945 < 8; v1945 += 1) {	// L2296
    for (int v1946 = 0; v1946 < 8; v1946 += 1) {	// L2297
      for (int v1947 = 0; v1947 < 8; v1947 += 1) {	// L2298
        #pragma HLS pipeline II=1
        float v1948 = v1940[v1945][v1946][v1947];	// L2299
        v1941[(v1945 + (v1942 * 8))][(v1946 + (v1943 * 8))][(v1947 + (v1944 * 8))] = v1948;	// L2300
      }
    }
  }
}

void forward_node151(
  float v1949[8][8],
  float v1950[8][8][8],
  float v1951[8][8][8],
  float v1952[8][8][8],
  float v1953[8][8][8],
  int v1954,
  int v1955,
  int v1956
) {	// L2306
  #pragma HLS inline
  for (int v1957 = 0; v1957 < 8; v1957 += 1) {	// L2308
    for (int v1958 = 0; v1958 < 8; v1958 += 1) {	// L2309
      for (int v1959 = 0; v1959 < 8; v1959 += 1) {	// L2310
        for (int v1960 = 0; v1960 < 8; v1960 += 2) {	// L2311
          #pragma HLS pipeline II=1
          float v1961 = v1950[v1958][v1959][v1960];	// L2312
          float v1962 = v1952[v1958][v1959][v1960];	// L2313
          float v1963 = v1953[v1958][v1959][v1960];	// L2314
          float v1964 = (v1957 == 0) ? v1962 : v1963;	// L2315
          float v1965 = ((v1957 + (v1955 * 8)) == 0 && v1954 == 0 && v1956 == 0) ? v1961 : v1964;	// L2316
          float v1966 = v1951[v1957][v1959][v1960];	// L2317
          float v1967 = v1949[v1958][v1957];	// L2318
          float v1968 = v1966 * v1967;	// L2319
          float v1969 = v1965 + v1968;	// L2320
          bool v1970 = v1969 > (float)0.000000;	// L2321
          float v1971 = v1970 ? v1969 : (float)0.000000;	// L2322
          float v1972 = ((((-v1957) + (v1955 * -8)) + 63) == 0 && ((-v1954) + 2) == 0 && ((-v1956) + 2) == 0) ? v1971 : v1969;	// L2323
          v1953[v1958][v1959][v1960] = v1972;	// L2324
          float v1973 = v1950[v1958][v1959][(v1960 + 1)];	// L2325
          float v1974 = v1952[v1958][v1959][(v1960 + 1)];	// L2326
          float v1975 = v1953[v1958][v1959][(v1960 + 1)];	// L2327
          float v1976 = (v1957 == 0) ? v1974 : v1975;	// L2328
          float v1977 = ((v1957 + (v1955 * 8)) == 0 && v1954 == 0 && v1956 == 0) ? v1973 : v1976;	// L2329
          float v1978 = v1951[v1957][v1959][(v1960 + 1)];	// L2330
          float v1979 = v1978 * v1967;	// L2331
          float v1980 = v1977 + v1979;	// L2332
          bool v1981 = v1980 > (float)0.000000;	// L2333
          float v1982 = v1981 ? v1980 : (float)0.000000;	// L2334
          float v1983 = ((((-v1957) + (v1955 * -8)) + 63) == 0 && ((-v1954) + 2) == 0 && ((-v1956) + 2) == 0) ? v1982 : v1980;	// L2335
          v1953[v1958][v1959][(v1960 + 1)] = v1983;	// L2336
        }
      }
    }
  }
}

void forward_node152(
  float v1984[64][64][3][3],
  float v1985[8][8],
  int v1986,
  int v1987,
  int v1988,
  int v1989
) {	// L2343
  #pragma HLS inline
  for (int v1990 = 0; v1990 < 8; v1990 += 1) {	// L2344
    for (int v1991 = 0; v1991 < 8; v1991 += 1) {	// L2345
      #pragma HLS pipeline II=1
      float v1992 = v1984[(v1990 + (v1988 * 8))][(v1991 + (v1989 * 8))][v1986][v1987];	// L2346
      v1985[v1990][v1991] = v1992;	// L2347
    }
  }
}

void forward_node153(
  float v1993[64][56][56],
  float v1994[8][8][8],
  int v1995,
  int v1996,
  int v1997,
  int v1998,
  int v1999
) {	// L2352
  #pragma HLS inline
  for (int v2000 = 0; v2000 < 8; v2000 += 1) {	// L2353
    for (int v2001 = 0; v2001 < 8; v2001 += 1) {	// L2354
      for (int v2002 = 0; v2002 < 8; v2002 += 2) {	// L2355
        #pragma HLS pipeline II=1
        float v2003 = v1993[(v2000 + (v1995 * 8))][(((v2001 + v1996) + (v1997 * 8)) - 1)][(((v2002 + v1998) + (v1999 * 8)) - 1)];	// L2356
        v1994[v2000][v2001][v2002] = v2003;	// L2357
        float v2004 = v1993[(v2000 + (v1995 * 8))][(((v2001 + v1996) + (v1997 * 8)) - 1)][((v2002 + v1998) + (v1999 * 8))];	// L2358
        v1994[v2000][v2001][(v2002 + 1)] = v2004;	// L2359
      }
    }
  }
}

void forward_node154(
  float v2005[64][56][56],
  float v2006[8][8][8],
  int v2007,
  int v2008,
  int v2009
) {	// L2365
  #pragma HLS inline
  for (int v2010 = 0; v2010 < 8; v2010 += 1) {	// L2366
    for (int v2011 = 0; v2011 < 8; v2011 += 1) {	// L2367
      for (int v2012 = 0; v2012 < 8; v2012 += 1) {	// L2368
        #pragma HLS pipeline II=1
        float v2013 = v2005[(v2010 + (v2007 * 8))][(v2011 + (v2008 * 8))][(v2012 + (v2009 * 8))];	// L2369
        v2006[v2010][v2011][v2012] = v2013;	// L2370
      }
    }
  }
}

void forward_node155(
  float v2014[64][56][56],
  float v2015[8][8][8],
  int v2016,
  int v2017,
  int v2018
) {	// L2376
  #pragma HLS inline
  for (int v2019 = 0; v2019 < 8; v2019 += 1) {	// L2377
    for (int v2020 = 0; v2020 < 8; v2020 += 1) {	// L2378
      for (int v2021 = 0; v2021 < 8; v2021 += 1) {	// L2379
        #pragma HLS pipeline II=1
        float v2022 = v2014[(v2019 + (v2016 * 8))][(v2020 + (v2017 * 8))][(v2021 + (v2018 * 8))];	// L2380
        v2015[v2019][v2020][v2021] = v2022;	// L2381
      }
    }
  }
}

void forward_node149(
  float v2023[64][56][56],
  float v2024[64][64][3][3],
  float v2025[64][56][56],
  float v2026[64][56][56],
  float v2027[64][56][56]
) {	// L2387
  for (int v2028 = 0; v2028 < 28224; v2028 += 1) {	// L2388
    #pragma HLS dataflow
    int v2029 = (v2028 % 7);	// L2389
    int v2030 = ((v2028 / 7) % 7);	// L2390
    int v2031 = (((v2028 / 7) / 7) % 8);	// L2391
    int v2032 = ((((v2028 / 7) / 7) / 8) % 3);	// L2392
    int v2033 = (((((v2028 / 7) / 7) / 8) / 3) % 3);	// L2393
    int v2034 = (((((v2028 / 7) / 7) / 8) / 3) / 3);	// L2394
    float v2035[8][8];	// L2395
    #pragma HLS bind_storage variable=v2035 type=ram_t2p impl=bram

    float v2036[8][8][8];	// L2396
    #pragma HLS array_partition variable=v2036 cyclic factor=2 dim=3
    #pragma HLS bind_storage variable=v2036 type=ram_2p impl=lutram

    float v2037[8][8][8];	// L2397
    #pragma HLS array_partition variable=v2037 cyclic factor=2 dim=3
    #pragma HLS bind_storage variable=v2037 type=ram_2p impl=lutram

    float v2038[8][8][8];	// L2398
    #pragma HLS array_partition variable=v2038 cyclic factor=2 dim=3
    #pragma HLS bind_storage variable=v2038 type=ram_2p impl=lutram

    forward_node155(v2025, v2038, v2031, v2030, v2029);	// L2399
    forward_node154(v2026, v2037, v2031, v2030, v2029);	// L2400
    forward_node153(v2023, v2036, v2034, v2033, v2030, v2032, v2029);	// L2401
    forward_node152(v2024, v2035, v2033, v2032, v2031, v2034);	// L2402
    float v2039[8][8][8];	// L2403
    #pragma HLS array_partition variable=v2039 cyclic factor=2 dim=3
    #pragma HLS bind_storage variable=v2039 type=ram_2p impl=lutram

    forward_node151(v2035, v2038, v2036, v2037, v2039, v2033, v2034, v2032);	// L2404
    forward_node150(v2039, v2027, v2031, v2030, v2029);	// L2405
  }
}

void forward_node157(
  float v2040[8][8][8],
  float v2041[64][56][56],
  int v2042,
  int v2043,
  int v2044
) {	// L2409
  #pragma HLS inline
  for (int v2045 = 0; v2045 < 8; v2045 += 1) {	// L2410
    for (int v2046 = 0; v2046 < 8; v2046 += 1) {	// L2411
      for (int v2047 = 0; v2047 < 8; v2047 += 2) {	// L2412
        #pragma HLS pipeline II=1
        float v2048 = v2040[v2045][v2046][v2047];	// L2413
        v2041[(v2045 + (v2042 * 8))][(v2046 + (v2043 * 8))][(v2047 + (v2044 * 8))] = v2048;	// L2414
        float v2049 = v2040[v2045][v2046][(v2047 + 1)];	// L2415
        v2041[(v2045 + (v2042 * 8))][(v2046 + (v2043 * 8))][((v2047 + (v2044 * 8)) + 1)] = v2049;	// L2416
      }
    }
  }
}

void forward_node158(
  float v2050[8][8][8],
  float v2051[8][8][8],
  float v2052[8][8][8]
) {	// L2422
  #pragma HLS inline
  for (int v2053 = 0; v2053 < 8; v2053 += 1) {	// L2423
    for (int v2054 = 0; v2054 < 8; v2054 += 1) {	// L2424
      for (int v2055 = 0; v2055 < 8; v2055 += 2) {	// L2425
        #pragma HLS pipeline II=1
        float v2056 = v2050[v2053][v2054][v2055];	// L2426
        float v2057 = v2051[v2053][v2054][v2055];	// L2427
        float v2058 = max(v2057, v2056);	// L2428
        v2052[v2053][v2054][v2055] = v2058;	// L2429
        float v2059 = v2050[v2053][v2054][(v2055 + 1)];	// L2430
        float v2060 = v2051[v2053][v2054][(v2055 + 1)];	// L2431
        float v2061 = max(v2060, v2059);	// L2432
        v2052[v2053][v2054][(v2055 + 1)] = v2061;	// L2433
      }
    }
  }
}

void forward_node159(
  float v2062[64][56][56],
  float v2063[8][8][8],
  int v2064,
  int v2065,
  int v2066
) {	// L2439
  #pragma HLS inline
  for (int v2067 = 0; v2067 < 8; v2067 += 1) {	// L2440
    for (int v2068 = 0; v2068 < 8; v2068 += 1) {	// L2441
      for (int v2069 = 0; v2069 < 8; v2069 += 2) {	// L2442
        #pragma HLS pipeline II=1
        float v2070 = v2062[(v2067 + (v2064 * 8))][(v2068 + (v2065 * 8))][(v2069 + (v2066 * 8))];	// L2443
        v2063[v2067][v2068][v2069] = v2070;	// L2444
        float v2071 = v2062[(v2067 + (v2064 * 8))][(v2068 + (v2065 * 8))][((v2069 + (v2066 * 8)) + 1)];	// L2445
        v2063[v2067][v2068][(v2069 + 1)] = v2071;	// L2446
      }
    }
  }
}

void forward_node160(
  float v2072[64][112][112],
  float v2073[8][8][8],
  int v2074,
  int v2075,
  int v2076,
  int v2077,
  int v2078
) {	// L2452
  #pragma HLS inline
  for (int v2079 = 0; v2079 < 8; v2079 += 1) {	// L2453
    for (int v2080 = 0; v2080 < 8; v2080 += 1) {	// L2454
      for (int v2081 = 0; v2081 < 8; v2081 += 2) {	// L2455
        #pragma HLS pipeline II=1
        float v2082 = v2072[(v2079 + (v2074 * 8))][((((v2080 * 2) + v2075) + (v2076 * 16)) - 1)][((((v2081 * 2) + v2077) + (v2078 * 16)) - 1)];	// L2456
        v2073[v2079][v2080][v2081] = v2082;	// L2457
        float v2083 = v2072[(v2079 + (v2074 * 8))][((((v2080 * 2) + v2075) + (v2076 * 16)) - 1)][((((v2081 * 2) + v2077) + (v2078 * 16)) + 1)];	// L2458
        v2073[v2079][v2080][(v2081 + 1)] = v2083;	// L2459
      }
    }
  }
}

void forward_node156(
  float v2084[64][112][112],
  float v2085[64][56][56],
  float v2086[64][56][56]
) {	// L2465
  for (int v2087 = 0; v2087 < 3528; v2087 += 1) {	// L2466
    #pragma HLS dataflow
    int v2088 = (v2087 % 7);	// L2467
    int v2089 = ((v2087 / 7) % 7);	// L2468
    int v2090 = (((v2087 / 7) / 7) % 8);	// L2469
    int v2091 = ((((v2087 / 7) / 7) / 8) % 3);	// L2470
    int v2092 = ((((v2087 / 7) / 7) / 8) / 3);	// L2471
    float v2093[8][8][8];	// L2472
    #pragma HLS array_partition variable=v2093 cyclic factor=2 dim=3
    #pragma HLS bind_storage variable=v2093 type=ram_2p impl=lutram

    float v2094[8][8][8];	// L2473
    #pragma HLS array_partition variable=v2094 cyclic factor=2 dim=3
    #pragma HLS bind_storage variable=v2094 type=ram_2p impl=lutram

    forward_node160(v2084, v2094, v2090, v2092, v2089, v2091, v2088);	// L2474
    forward_node159(v2085, v2093, v2090, v2089, v2088);	// L2475
    float v2095[8][8][8];	// L2476
    #pragma HLS array_partition variable=v2095 cyclic factor=2 dim=3
    #pragma HLS bind_storage variable=v2095 type=ram_2p impl=lutram

    forward_node158(v2094, v2093, v2095);	// L2477
    forward_node157(v2095, v2086, v2090, v2089, v2088);	// L2478
  }
}

void forward_node162(
  float v2096[8][8][8],
  float v2097[64][112][112],
  int v2098,
  int v2099,
  int v2100
) {	// L2482
  #pragma HLS inline
  for (int v2101 = 0; v2101 < 8; v2101 += 1) {	// L2483
    for (int v2102 = 0; v2102 < 8; v2102 += 2) {	// L2484
      for (int v2103 = 0; v2103 < 8; v2103 += 4) {	// L2485
        #pragma HLS pipeline II=1
        float v2104 = v2096[v2101][v2102][v2103];	// L2486
        v2097[(v2101 + (v2098 * 8))][(v2102 + (v2099 * 8))][(v2103 + (v2100 * 8))] = v2104;	// L2487
        float v2105 = v2096[v2101][v2102][(v2103 + 1)];	// L2488
        v2097[(v2101 + (v2098 * 8))][(v2102 + (v2099 * 8))][((v2103 + (v2100 * 8)) + 1)] = v2105;	// L2489
        float v2106 = v2096[v2101][v2102][(v2103 + 2)];	// L2490
        v2097[(v2101 + (v2098 * 8))][(v2102 + (v2099 * 8))][((v2103 + (v2100 * 8)) + 2)] = v2106;	// L2491
        float v2107 = v2096[v2101][v2102][(v2103 + 3)];	// L2492
        v2097[(v2101 + (v2098 * 8))][(v2102 + (v2099 * 8))][((v2103 + (v2100 * 8)) + 3)] = v2107;	// L2493
        float v2108 = v2096[v2101][(v2102 + 1)][v2103];	// L2494
        v2097[(v2101 + (v2098 * 8))][((v2102 + (v2099 * 8)) + 1)][(v2103 + (v2100 * 8))] = v2108;	// L2495
        float v2109 = v2096[v2101][(v2102 + 1)][(v2103 + 1)];	// L2496
        v2097[(v2101 + (v2098 * 8))][((v2102 + (v2099 * 8)) + 1)][((v2103 + (v2100 * 8)) + 1)] = v2109;	// L2497
        float v2110 = v2096[v2101][(v2102 + 1)][(v2103 + 2)];	// L2498
        v2097[(v2101 + (v2098 * 8))][((v2102 + (v2099 * 8)) + 1)][((v2103 + (v2100 * 8)) + 2)] = v2110;	// L2499
        float v2111 = v2096[v2101][(v2102 + 1)][(v2103 + 3)];	// L2500
        v2097[(v2101 + (v2098 * 8))][((v2102 + (v2099 * 8)) + 1)][((v2103 + (v2100 * 8)) + 3)] = v2111;	// L2501
      }
    }
  }
}

void forward_node163(
  float v2112[8][8][8],
  float v2113[8][8][8],
  float v2114[8][8][8],
  int v2115,
  int v2116
) {	// L2507
  #pragma HLS inline
  for (int v2117 = 0; v2117 < 8; v2117 += 1) {	// L2509
    for (int v2118 = 0; v2118 < 8; v2118 += 1) {	// L2510
      for (int v2119 = 0; v2119 < 8; v2119 += 2) {	// L2511
        #pragma HLS pipeline II=1
        float v2120 = v2112[v2117][v2118][v2119];	// L2512
        float v2121 = v2113[v2117][v2118][v2119];	// L2513
        float v2122 = max(v2121, v2120);	// L2514
        bool v2123 = v2122 > (float)0.000000;	// L2515
        float v2124 = v2123 ? v2122 : (float)0.000000;	// L2516
        float v2125 = (((-v2115) + 1) == 0 && ((-v2116) + 1) == 0) ? v2124 : v2122;	// L2517
        v2114[v2117][v2118][v2119] = v2125;	// L2518
        float v2126 = v2112[v2117][v2118][(v2119 + 1)];	// L2519
        float v2127 = v2113[v2117][v2118][(v2119 + 1)];	// L2520
        float v2128 = max(v2127, v2126);	// L2521
        bool v2129 = v2128 > (float)0.000000;	// L2522
        float v2130 = v2129 ? v2128 : (float)0.000000;	// L2523
        float v2131 = (((-v2115) + 1) == 0 && ((-v2116) + 1) == 0) ? v2130 : v2128;	// L2524
        v2114[v2117][v2118][(v2119 + 1)] = v2131;	// L2525
      }
    }
  }
}

void forward_node164(
  float v2132[64][112][112],
  float v2133[8][8][8],
  int v2134,
  int v2135,
  int v2136
) {	// L2531
  #pragma HLS inline
  for (int v2137 = 0; v2137 < 8; v2137 += 1) {	// L2532
    for (int v2138 = 0; v2138 < 8; v2138 += 2) {	// L2533
      for (int v2139 = 0; v2139 < 8; v2139 += 4) {	// L2534
        #pragma HLS pipeline II=1
        float v2140 = v2132[(v2137 + (v2134 * 8))][(v2138 + (v2135 * 8))][(v2139 + (v2136 * 8))];	// L2535
        v2133[v2137][v2138][v2139] = v2140;	// L2536
        float v2141 = v2132[(v2137 + (v2134 * 8))][(v2138 + (v2135 * 8))][((v2139 + (v2136 * 8)) + 1)];	// L2537
        v2133[v2137][v2138][(v2139 + 1)] = v2141;	// L2538
        float v2142 = v2132[(v2137 + (v2134 * 8))][(v2138 + (v2135 * 8))][((v2139 + (v2136 * 8)) + 2)];	// L2539
        v2133[v2137][v2138][(v2139 + 2)] = v2142;	// L2540
        float v2143 = v2132[(v2137 + (v2134 * 8))][(v2138 + (v2135 * 8))][((v2139 + (v2136 * 8)) + 3)];	// L2541
        v2133[v2137][v2138][(v2139 + 3)] = v2143;	// L2542
        float v2144 = v2132[(v2137 + (v2134 * 8))][((v2138 + (v2135 * 8)) + 1)][(v2139 + (v2136 * 8))];	// L2543
        v2133[v2137][(v2138 + 1)][v2139] = v2144;	// L2544
        float v2145 = v2132[(v2137 + (v2134 * 8))][((v2138 + (v2135 * 8)) + 1)][((v2139 + (v2136 * 8)) + 1)];	// L2545
        v2133[v2137][(v2138 + 1)][(v2139 + 1)] = v2145;	// L2546
        float v2146 = v2132[(v2137 + (v2134 * 8))][((v2138 + (v2135 * 8)) + 1)][((v2139 + (v2136 * 8)) + 2)];	// L2547
        v2133[v2137][(v2138 + 1)][(v2139 + 2)] = v2146;	// L2548
        float v2147 = v2132[(v2137 + (v2134 * 8))][((v2138 + (v2135 * 8)) + 1)][((v2139 + (v2136 * 8)) + 3)];	// L2549
        v2133[v2137][(v2138 + 1)][(v2139 + 3)] = v2147;	// L2550
      }
    }
  }
}

void forward_node165(
  float v2148[64][224][224],
  float v2149[8][8][8],
  int v2150,
  int v2151,
  int v2152,
  int v2153,
  int v2154
) {	// L2556
  #pragma HLS inline
  for (int v2155 = 0; v2155 < 8; v2155 += 2) {	// L2557
    for (int v2156 = 0; v2156 < 8; v2156 += 1) {	// L2558
      for (int v2157 = 0; v2157 < 8; v2157 += 1) {	// L2559
        #pragma HLS pipeline II=1
        float v2158 = v2148[(v2155 + (v2150 * 8))][(((v2156 * 2) + v2151) + (v2152 * 16))][(((v2157 * 2) + v2153) + (v2154 * 16))];	// L2560
        v2149[v2155][v2156][v2157] = v2158;	// L2561
        float v2159 = v2148[((v2155 + (v2150 * 8)) + 1)][(((v2156 * 2) + v2151) + (v2152 * 16))][(((v2157 * 2) + v2153) + (v2154 * 16))];	// L2562
        v2149[(v2155 + 1)][v2156][v2157] = v2159;	// L2563
      }
    }
  }
}

void forward_node161(
  float v2160[64][224][224],
  float v2161[64][112][112],
  float v2162[64][112][112]
) {	// L2569
  for (int v2163 = 0; v2163 < 6272; v2163 += 1) {	// L2570
    #pragma HLS dataflow
    int v2164 = (v2163 % 14);	// L2571
    int v2165 = ((v2163 / 14) % 14);	// L2572
    int v2166 = (((v2163 / 14) / 14) % 8);	// L2573
    int v2167 = ((((v2163 / 14) / 14) / 8) % 2);	// L2574
    int v2168 = ((((v2163 / 14) / 14) / 8) / 2);	// L2575
    float v2169[8][8][8];	// L2576
    #pragma HLS array_partition variable=v2169 cyclic factor=2 dim=2
    #pragma HLS array_partition variable=v2169 cyclic factor=4 dim=3
    #pragma HLS bind_storage variable=v2169 type=ram_2p impl=lutram

    float v2170[8][8][8];	// L2577
    #pragma HLS array_partition variable=v2170 cyclic factor=2 dim=1
    #pragma HLS array_partition variable=v2170 cyclic factor=2 dim=3
    #pragma HLS bind_storage variable=v2170 type=ram_2p impl=lutram

    forward_node165(v2160, v2170, v2166, v2168, v2165, v2167, v2164);	// L2578
    forward_node164(v2161, v2169, v2166, v2165, v2164);	// L2579
    float v2171[8][8][8];	// L2580
    #pragma HLS array_partition variable=v2171 cyclic factor=2 dim=2
    #pragma HLS array_partition variable=v2171 cyclic factor=4 dim=3
    #pragma HLS bind_storage variable=v2171 type=ram_2p impl=lutram

    forward_node163(v2170, v2169, v2171, v2168, v2167);	// L2581
    forward_node162(v2171, v2162, v2166, v2165, v2164);	// L2582
  }
}

void forward_node167(
  float v2172[8][8][8],
  float v2173[64][224][224],
  int v2174,
  int v2175,
  int v2176
) {	// L2586
  #pragma HLS inline
  for (int v2177 = 0; v2177 < 8; v2177 += 2) {	// L2587
    for (int v2178 = 0; v2178 < 8; v2178 += 2) {	// L2588
      for (int v2179 = 0; v2179 < 8; v2179 += 2) {	// L2589
        #pragma HLS pipeline II=1
        float v2180 = v2172[v2177][v2178][v2179];	// L2590
        v2173[(v2177 + (v2174 * 8))][(v2178 + (v2175 * 8))][(v2179 + (v2176 * 8))] = v2180;	// L2591
        float v2181 = v2172[v2177][v2178][(v2179 + 1)];	// L2592
        v2173[(v2177 + (v2174 * 8))][(v2178 + (v2175 * 8))][((v2179 + (v2176 * 8)) + 1)] = v2181;	// L2593
        float v2182 = v2172[v2177][(v2178 + 1)][v2179];	// L2594
        v2173[(v2177 + (v2174 * 8))][((v2178 + (v2175 * 8)) + 1)][(v2179 + (v2176 * 8))] = v2182;	// L2595
        float v2183 = v2172[v2177][(v2178 + 1)][(v2179 + 1)];	// L2596
        v2173[(v2177 + (v2174 * 8))][((v2178 + (v2175 * 8)) + 1)][((v2179 + (v2176 * 8)) + 1)] = v2183;	// L2597
        float v2184 = v2172[(v2177 + 1)][v2178][v2179];	// L2598
        v2173[((v2177 + (v2174 * 8)) + 1)][(v2178 + (v2175 * 8))][(v2179 + (v2176 * 8))] = v2184;	// L2599
        float v2185 = v2172[(v2177 + 1)][v2178][(v2179 + 1)];	// L2600
        v2173[((v2177 + (v2174 * 8)) + 1)][(v2178 + (v2175 * 8))][((v2179 + (v2176 * 8)) + 1)] = v2185;	// L2601
        float v2186 = v2172[(v2177 + 1)][(v2178 + 1)][v2179];	// L2602
        v2173[((v2177 + (v2174 * 8)) + 1)][((v2178 + (v2175 * 8)) + 1)][(v2179 + (v2176 * 8))] = v2186;	// L2603
        float v2187 = v2172[(v2177 + 1)][(v2178 + 1)][(v2179 + 1)];	// L2604
        v2173[((v2177 + (v2174 * 8)) + 1)][((v2178 + (v2175 * 8)) + 1)][((v2179 + (v2176 * 8)) + 1)] = v2187;	// L2605
      }
    }
  }
}

void forward_node168(
  float v2188[8][8],
  float v2189[8],
  float v2190[8][8][8],
  float v2191[8][8][8]
) {	// L2611
  #pragma HLS inline
  for (int v2192 = 0; v2192 < 8; v2192 += 2) {	// L2612
    for (int v2193 = 0; v2193 < 8; v2193 += 2) {	// L2613
      for (int v2194 = 0; v2194 < 8; v2194 += 2) {	// L2614
        #pragma HLS pipeline II=1
        float v2195 = v2188[v2193][v2194];	// L2615
        float v2196 = v2189[v2192];	// L2616
        float v2197 = v2190[v2192][v2193][v2194];	// L2617
        float v2198 = v2195 * v2196;	// L2618
        float v2199 = v2197 + v2198;	// L2619
        v2191[v2192][v2193][v2194] = v2199;	// L2620
        float v2200 = v2188[v2193][(v2194 + 1)];	// L2621
        float v2201 = v2190[v2192][v2193][(v2194 + 1)];	// L2622
        float v2202 = v2200 * v2196;	// L2623
        float v2203 = v2201 + v2202;	// L2624
        v2191[v2192][v2193][(v2194 + 1)] = v2203;	// L2625
        float v2204 = v2188[(v2193 + 1)][v2194];	// L2626
        float v2205 = v2190[v2192][(v2193 + 1)][v2194];	// L2627
        float v2206 = v2204 * v2196;	// L2628
        float v2207 = v2205 + v2206;	// L2629
        v2191[v2192][(v2193 + 1)][v2194] = v2207;	// L2630
        float v2208 = v2188[(v2193 + 1)][(v2194 + 1)];	// L2631
        float v2209 = v2190[v2192][(v2193 + 1)][(v2194 + 1)];	// L2632
        float v2210 = v2208 * v2196;	// L2633
        float v2211 = v2209 + v2210;	// L2634
        v2191[v2192][(v2193 + 1)][(v2194 + 1)] = v2211;	// L2635
        float v2212 = v2189[(v2192 + 1)];	// L2636
        float v2213 = v2190[(v2192 + 1)][v2193][v2194];	// L2637
        float v2214 = v2195 * v2212;	// L2638
        float v2215 = v2213 + v2214;	// L2639
        v2191[(v2192 + 1)][v2193][v2194] = v2215;	// L2640
        float v2216 = v2190[(v2192 + 1)][v2193][(v2194 + 1)];	// L2641
        float v2217 = v2200 * v2212;	// L2642
        float v2218 = v2216 + v2217;	// L2643
        v2191[(v2192 + 1)][v2193][(v2194 + 1)] = v2218;	// L2644
        float v2219 = v2190[(v2192 + 1)][(v2193 + 1)][v2194];	// L2645
        float v2220 = v2204 * v2212;	// L2646
        float v2221 = v2219 + v2220;	// L2647
        v2191[(v2192 + 1)][(v2193 + 1)][v2194] = v2221;	// L2648
        float v2222 = v2190[(v2192 + 1)][(v2193 + 1)][(v2194 + 1)];	// L2649
        float v2223 = v2208 * v2212;	// L2650
        float v2224 = v2222 + v2223;	// L2651
        v2191[(v2192 + 1)][(v2193 + 1)][(v2194 + 1)] = v2224;	// L2652
      }
    }
  }
}

void forward_node169(
  float v2225[64][224][224],
  float v2226[8][8][8],
  int v2227,
  int v2228,
  int v2229
) {	// L2658
  #pragma HLS inline
  for (int v2230 = 0; v2230 < 8; v2230 += 2) {	// L2659
    for (int v2231 = 0; v2231 < 8; v2231 += 2) {	// L2660
      for (int v2232 = 0; v2232 < 8; v2232 += 2) {	// L2661
        #pragma HLS pipeline II=1
        float v2233 = v2225[(v2230 + (v2227 * 8))][(v2231 + (v2228 * 8))][(v2232 + (v2229 * 8))];	// L2662
        v2226[v2230][v2231][v2232] = v2233;	// L2663
        float v2234 = v2225[(v2230 + (v2227 * 8))][(v2231 + (v2228 * 8))][((v2232 + (v2229 * 8)) + 1)];	// L2664
        v2226[v2230][v2231][(v2232 + 1)] = v2234;	// L2665
        float v2235 = v2225[(v2230 + (v2227 * 8))][((v2231 + (v2228 * 8)) + 1)][(v2232 + (v2229 * 8))];	// L2666
        v2226[v2230][(v2231 + 1)][v2232] = v2235;	// L2667
        float v2236 = v2225[(v2230 + (v2227 * 8))][((v2231 + (v2228 * 8)) + 1)][((v2232 + (v2229 * 8)) + 1)];	// L2668
        v2226[v2230][(v2231 + 1)][(v2232 + 1)] = v2236;	// L2669
        float v2237 = v2225[((v2230 + (v2227 * 8)) + 1)][(v2231 + (v2228 * 8))][(v2232 + (v2229 * 8))];	// L2670
        v2226[(v2230 + 1)][v2231][v2232] = v2237;	// L2671
        float v2238 = v2225[((v2230 + (v2227 * 8)) + 1)][(v2231 + (v2228 * 8))][((v2232 + (v2229 * 8)) + 1)];	// L2672
        v2226[(v2230 + 1)][v2231][(v2232 + 1)] = v2238;	// L2673
        float v2239 = v2225[((v2230 + (v2227 * 8)) + 1)][((v2231 + (v2228 * 8)) + 1)][(v2232 + (v2229 * 8))];	// L2674
        v2226[(v2230 + 1)][(v2231 + 1)][v2232] = v2239;	// L2675
        float v2240 = v2225[((v2230 + (v2227 * 8)) + 1)][((v2231 + (v2228 * 8)) + 1)][((v2232 + (v2229 * 8)) + 1)];	// L2676
        v2226[(v2230 + 1)][(v2231 + 1)][(v2232 + 1)] = v2240;	// L2677
      }
    }
  }
}

void forward_node170(
  float v2241[64][3][7][7],
  float v2242[8],
  int v2243,
  int v2244,
  int v2245,
  int v2246
) {	// L2683
  #pragma HLS inline
  for (int v2247 = 0; v2247 < 8; v2247 += 1) {	// L2684
    #pragma HLS pipeline II=1
    float v2248 = v2241[(v2247 + (v2246 * 8))][v2243][v2244][v2245];	// L2685
    v2242[v2247] = v2248;	// L2686
  }
}

void forward_node171(
  float v2249[3][224][224],
  float v2250[8][8],
  int v2251,
  int v2252,
  int v2253,
  int v2254,
  int v2255
) {	// L2690
  #pragma HLS inline
  for (int v2256 = 0; v2256 < 8; v2256 += 1) {	// L2691
    for (int v2257 = 0; v2257 < 8; v2257 += 1) {	// L2692
      #pragma HLS pipeline II=1
      float v2258 = v2249[v2251][(((v2256 + v2252) + (v2253 * 8)) - 3)][(((v2257 + v2254) + (v2255 * 8)) - 3)];	// L2693
      v2250[v2256][v2257] = v2258;	// L2694
    }
  }
}

void forward_node166(
  float v2259[3][224][224],
  float v2260[64][3][7][7],
  float v2261[64][224][224],
  float v2262[64][224][224]
) {	// L2699
  for (int v2263 = 0; v2263 < 921984; v2263 += 1) {	// L2700
    #pragma HLS dataflow
    int v2264 = (v2263 % 28);	// L2701
    int v2265 = ((v2263 / 28) % 28);	// L2702
    int v2266 = (((v2263 / 28) / 28) % 8);	// L2703
    int v2267 = ((((v2263 / 28) / 28) / 8) % 7);	// L2704
    int v2268 = (((((v2263 / 28) / 28) / 8) / 7) % 7);	// L2705
    int v2269 = (((((v2263 / 28) / 28) / 8) / 7) / 7);	// L2706
    float v2270[8][8][8];	// L2707
    #pragma HLS array_partition variable=v2270 cyclic factor=2 dim=1
    #pragma HLS array_partition variable=v2270 cyclic factor=2 dim=2
    #pragma HLS array_partition variable=v2270 cyclic factor=2 dim=3
    #pragma HLS bind_storage variable=v2270 type=ram_2p impl=lutram

    float v2271[8];	// L2708
    #pragma HLS array_partition variable=v2271 cyclic factor=2 dim=1
    #pragma HLS bind_storage variable=v2271 type=ram_2p impl=lutram

    float v2272[8][8];	// L2709
    #pragma HLS array_partition variable=v2272 cyclic factor=2 dim=1
    #pragma HLS array_partition variable=v2272 cyclic factor=2 dim=2
    #pragma HLS bind_storage variable=v2272 type=ram_2p impl=lutram

    forward_node171(v2259, v2272, v2269, v2268, v2265, v2267, v2264);	// L2710
    forward_node170(v2260, v2271, v2269, v2268, v2267, v2266);	// L2711
    forward_node169(v2261, v2270, v2266, v2265, v2264);	// L2712
    float v2273[8][8][8];	// L2713
    #pragma HLS array_partition variable=v2273 cyclic factor=2 dim=1
    #pragma HLS array_partition variable=v2273 cyclic factor=2 dim=2
    #pragma HLS array_partition variable=v2273 cyclic factor=2 dim=3
    #pragma HLS bind_storage variable=v2273 type=ram_2p impl=lutram

    forward_node168(v2272, v2271, v2270, v2273);	// L2714
    forward_node167(v2273, v2262, v2266, v2265, v2264);	// L2715
  }
}

/// This is top function.
void forward(
  float v2274[3][224][224],
  float v2275[1000],
  float v2276[64][3][7][7],
  float v2277[64][64][3][3],
  float v2278[64][64][3][3],
  float v2279[64][64][3][3],
  float v2280[64][64][3][3],
  float v2281[128][64][3][3],
  float v2282[128][128][3][3],
  float v2283[128][64],
  float v2284[128][128][3][3],
  float v2285[128][128][3][3],
  float v2286[256][128][3][3],
  float v2287[256][256][3][3],
  float v2288[256][128],
  float v2289[256][256][3][3],
  float v2290[256][256][3][3],
  float v2291[512][256][3][3],
  float v2292[512][512][3][3],
  float v2293[512][256],
  float v2294[512][512][3][3],
  float v2295[512][512][3][3],
  float v2296[512][1000],
  float v2297[64][224][224],
  float v2298[64][224][224],
  float v2299[64][224][224],
  float v2300[64][112][112],
  float v2301[64][112][112],
  float v2302[64][112][112],
  float v2303[64][56][56],
  float v2304[64][56][56],
  float v2305[64][56][56],
  float v2306[64][56][56],
  float v2307[64][56][56],
  float v2308[64][56][56],
  float v2309[64][56][56],
  float v2310[64][56][56],
  float v2311[64][56][56],
  float v2312[64][56][56],
  float v2313[64][56][56],
  float v2314[64][56][56],
  float v2315[64][56][56],
  float v2316[64][56][56],
  float v2317[64][56][56],
  float v2318[64][56][56],
  float v2319[64][56][56],
  float v2320[64][56][56],
  float v2321[64][56][56],
  float v2322[64][56][56],
  float v2323[64][56][56],
  float v2324[64][56][56],
  float v2325[64][56][56],
  float v2326[128][28][28],
  float v2327[128][28][28],
  float v2328[128][28][28],
  float v2329[128][28][28],
  float v2330[128][28][28],
  float v2331[128][28][28],
  float v2332[128][28][28],
  float v2333[128][28][28],
  float v2334[128][28][28],
  float v2335[128][28][28],
  float v2336[128][28][28],
  float v2337[128][28][28],
  float v2338[128][28][28],
  float v2339[128][28][28],
  float v2340[128][28][28],
  float v2341[128][28][28],
  float v2342[128][28][28],
  float v2343[128][28][28],
  float v2344[128][28][28],
  float v2345[128][28][28],
  float v2346[128][28][28],
  float v2347[128][28][28],
  float v2348[128][28][28],
  float v2349[128][28][28],
  float v2350[128][28][28],
  float v2351[256][14][14],
  float v2352[256][14][14],
  float v2353[256][14][14],
  float v2354[256][14][14],
  float v2355[256][14][14],
  float v2356[256][14][14],
  float v2357[256][14][14],
  float v2358[256][14][14],
  float v2359[256][14][14],
  float v2360[256][14][14],
  float v2361[256][14][14],
  float v2362[256][14][14],
  float v2363[256][14][14],
  float v2364[256][14][14],
  float v2365[256][14][14],
  float v2366[256][14][14],
  float v2367[256][14][14],
  float v2368[256][14][14],
  float v2369[256][14][14],
  float v2370[256][14][14],
  float v2371[256][14][14],
  float v2372[256][14][14],
  float v2373[256][14][14],
  float v2374[512][7][7],
  float v2375[512][7][7],
  float v2376[512][7][7],
  float v2377[512][7][7],
  float v2378[512][7][7],
  float v2379[512][7][7],
  float v2380[512][7][7],
  float v2381[512][7][7],
  float v2382[512][7][7],
  float v2383[512][7][7],
  float v2384[512][7][7],
  float v2385[512][7][7],
  float v2386[512][7][7],
  float v2387[512][7][7],
  float v2388[512][7][7],
  float v2389[512][7][7],
  float v2390[512][7][7],
  float v2391[512][7][7],
  float v2392[512][7][7],
  float v2393[512][7][7],
  float v2394[512][7][7],
  float v2395[512][7][7]
) {	// L2719
  #pragma HLS interface s_axilite port=return bundle=ctrl
  #pragma HLS interface m_axi offset=slave port=v2395 bundle=axi_121

  #pragma HLS interface m_axi offset=slave port=v2394 bundle=axi_120

  #pragma HLS interface m_axi offset=slave port=v2393 bundle=axi_119

  #pragma HLS interface m_axi offset=slave port=v2392 bundle=axi_118

  #pragma HLS interface m_axi offset=slave port=v2391 bundle=axi_117

  #pragma HLS interface m_axi offset=slave port=v2390 bundle=axi_116

  #pragma HLS interface m_axi offset=slave port=v2389 bundle=axi_115

  #pragma HLS interface m_axi offset=slave port=v2388 bundle=axi_114

  #pragma HLS interface m_axi offset=slave port=v2387 bundle=axi_113

  #pragma HLS interface m_axi offset=slave port=v2386 bundle=axi_112

  #pragma HLS interface m_axi offset=slave port=v2385 bundle=axi_111

  #pragma HLS interface m_axi offset=slave port=v2384 bundle=axi_110

  #pragma HLS interface m_axi offset=slave port=v2383 bundle=axi_109

  #pragma HLS interface m_axi offset=slave port=v2382 bundle=axi_108

  #pragma HLS interface m_axi offset=slave port=v2381 bundle=axi_107

  #pragma HLS interface m_axi offset=slave port=v2380 bundle=axi_106

  #pragma HLS interface m_axi offset=slave port=v2379 bundle=axi_105

  #pragma HLS interface m_axi offset=slave port=v2378 bundle=axi_104

  #pragma HLS interface m_axi offset=slave port=v2377 bundle=axi_103

  #pragma HLS interface m_axi offset=slave port=v2376 bundle=axi_102

  #pragma HLS interface m_axi offset=slave port=v2375 bundle=axi_101

  #pragma HLS interface m_axi offset=slave port=v2374 bundle=axi_100

  #pragma HLS interface m_axi offset=slave port=v2373 bundle=axi_99

  #pragma HLS interface m_axi offset=slave port=v2372 bundle=axi_98

  #pragma HLS interface m_axi offset=slave port=v2371 bundle=axi_97

  #pragma HLS interface m_axi offset=slave port=v2370 bundle=axi_96

  #pragma HLS interface m_axi offset=slave port=v2369 bundle=axi_95

  #pragma HLS interface m_axi offset=slave port=v2368 bundle=axi_94

  #pragma HLS interface m_axi offset=slave port=v2367 bundle=axi_93

  #pragma HLS interface m_axi offset=slave port=v2366 bundle=axi_92

  #pragma HLS interface m_axi offset=slave port=v2365 bundle=axi_91

  #pragma HLS interface m_axi offset=slave port=v2364 bundle=axi_90

  #pragma HLS interface m_axi offset=slave port=v2363 bundle=axi_89

  #pragma HLS interface m_axi offset=slave port=v2362 bundle=axi_88

  #pragma HLS interface m_axi offset=slave port=v2361 bundle=axi_87

  #pragma HLS interface m_axi offset=slave port=v2360 bundle=axi_86

  #pragma HLS interface m_axi offset=slave port=v2359 bundle=axi_85

  #pragma HLS interface m_axi offset=slave port=v2358 bundle=axi_84

  #pragma HLS interface m_axi offset=slave port=v2357 bundle=axi_83

  #pragma HLS interface m_axi offset=slave port=v2356 bundle=axi_82

  #pragma HLS interface m_axi offset=slave port=v2355 bundle=axi_81

  #pragma HLS interface m_axi offset=slave port=v2354 bundle=axi_80

  #pragma HLS interface m_axi offset=slave port=v2353 bundle=axi_79

  #pragma HLS interface m_axi offset=slave port=v2352 bundle=axi_78

  #pragma HLS interface m_axi offset=slave port=v2351 bundle=axi_77

  #pragma HLS interface m_axi offset=slave port=v2350 bundle=axi_76

  #pragma HLS interface m_axi offset=slave port=v2349 bundle=axi_75

  #pragma HLS interface m_axi offset=slave port=v2348 bundle=axi_74

  #pragma HLS interface m_axi offset=slave port=v2347 bundle=axi_73

  #pragma HLS interface m_axi offset=slave port=v2346 bundle=axi_72

  #pragma HLS interface m_axi offset=slave port=v2345 bundle=axi_71

  #pragma HLS interface m_axi offset=slave port=v2344 bundle=axi_70

  #pragma HLS interface m_axi offset=slave port=v2343 bundle=axi_69

  #pragma HLS interface m_axi offset=slave port=v2342 bundle=axi_68

  #pragma HLS interface m_axi offset=slave port=v2341 bundle=axi_67

  #pragma HLS interface m_axi offset=slave port=v2340 bundle=axi_66

  #pragma HLS interface m_axi offset=slave port=v2339 bundle=axi_65

  #pragma HLS interface m_axi offset=slave port=v2338 bundle=axi_64

  #pragma HLS interface m_axi offset=slave port=v2337 bundle=axi_63

  #pragma HLS interface m_axi offset=slave port=v2336 bundle=axi_62

  #pragma HLS interface m_axi offset=slave port=v2335 bundle=axi_61

  #pragma HLS interface m_axi offset=slave port=v2334 bundle=axi_60

  #pragma HLS interface m_axi offset=slave port=v2333 bundle=axi_59

  #pragma HLS interface m_axi offset=slave port=v2332 bundle=axi_58

  #pragma HLS interface m_axi offset=slave port=v2331 bundle=axi_57

  #pragma HLS interface m_axi offset=slave port=v2330 bundle=axi_56

  #pragma HLS interface m_axi offset=slave port=v2329 bundle=axi_55

  #pragma HLS interface m_axi offset=slave port=v2328 bundle=axi_54

  #pragma HLS interface m_axi offset=slave port=v2327 bundle=axi_53

  #pragma HLS interface m_axi offset=slave port=v2326 bundle=axi_52

  #pragma HLS interface m_axi offset=slave port=v2325 bundle=axi_51

  #pragma HLS interface m_axi offset=slave port=v2324 bundle=axi_50

  #pragma HLS interface m_axi offset=slave port=v2323 bundle=axi_49

  #pragma HLS interface m_axi offset=slave port=v2322 bundle=axi_48

  #pragma HLS interface m_axi offset=slave port=v2321 bundle=axi_47

  #pragma HLS interface m_axi offset=slave port=v2320 bundle=axi_46

  #pragma HLS interface m_axi offset=slave port=v2319 bundle=axi_45

  #pragma HLS interface m_axi offset=slave port=v2318 bundle=axi_44

  #pragma HLS interface m_axi offset=slave port=v2317 bundle=axi_43

  #pragma HLS interface m_axi offset=slave port=v2316 bundle=axi_42

  #pragma HLS interface m_axi offset=slave port=v2315 bundle=axi_41

  #pragma HLS interface m_axi offset=slave port=v2314 bundle=axi_40

  #pragma HLS interface m_axi offset=slave port=v2313 bundle=axi_39

  #pragma HLS interface m_axi offset=slave port=v2312 bundle=axi_38

  #pragma HLS interface m_axi offset=slave port=v2311 bundle=axi_37

  #pragma HLS interface m_axi offset=slave port=v2310 bundle=axi_36

  #pragma HLS interface m_axi offset=slave port=v2309 bundle=axi_35

  #pragma HLS interface m_axi offset=slave port=v2308 bundle=axi_34

  #pragma HLS interface m_axi offset=slave port=v2307 bundle=axi_33

  #pragma HLS interface m_axi offset=slave port=v2306 bundle=axi_32

  #pragma HLS interface m_axi offset=slave port=v2305 bundle=axi_31

  #pragma HLS interface m_axi offset=slave port=v2304 bundle=axi_30

  #pragma HLS interface m_axi offset=slave port=v2303 bundle=axi_29

  #pragma HLS interface m_axi offset=slave port=v2302 bundle=axi_28

  #pragma HLS interface m_axi offset=slave port=v2301 bundle=axi_27

  #pragma HLS interface m_axi offset=slave port=v2300 bundle=axi_26

  #pragma HLS interface m_axi offset=slave port=v2299 bundle=axi_25

  #pragma HLS interface m_axi offset=slave port=v2298 bundle=axi_24

  #pragma HLS interface m_axi offset=slave port=v2297 bundle=axi_23

  #pragma HLS interface m_axi offset=slave port=v2296 bundle=axi_22

  #pragma HLS interface m_axi offset=slave port=v2295 bundle=axi_21

  #pragma HLS interface m_axi offset=slave port=v2294 bundle=axi_20

  #pragma HLS interface m_axi offset=slave port=v2293 bundle=axi_19

  #pragma HLS interface m_axi offset=slave port=v2292 bundle=axi_18

  #pragma HLS interface m_axi offset=slave port=v2291 bundle=axi_17

  #pragma HLS interface m_axi offset=slave port=v2290 bundle=axi_16

  #pragma HLS interface m_axi offset=slave port=v2289 bundle=axi_15

  #pragma HLS interface m_axi offset=slave port=v2288 bundle=axi_14

  #pragma HLS interface m_axi offset=slave port=v2287 bundle=axi_13

  #pragma HLS interface m_axi offset=slave port=v2286 bundle=axi_12

  #pragma HLS interface m_axi offset=slave port=v2285 bundle=axi_11

  #pragma HLS interface m_axi offset=slave port=v2284 bundle=axi_10

  #pragma HLS interface m_axi offset=slave port=v2283 bundle=axi_9

  #pragma HLS interface m_axi offset=slave port=v2282 bundle=axi_8

  #pragma HLS interface m_axi offset=slave port=v2281 bundle=axi_7

  #pragma HLS interface m_axi offset=slave port=v2280 bundle=axi_6

  #pragma HLS interface m_axi offset=slave port=v2279 bundle=axi_5

  #pragma HLS interface m_axi offset=slave port=v2278 bundle=axi_4

  #pragma HLS interface m_axi offset=slave port=v2277 bundle=axi_3

  #pragma HLS interface m_axi offset=slave port=v2276 bundle=axi_2

  #pragma HLS interface bram storage_type=ram_2p storage_impl=lutram port=v2275 bundle=axi_1
  #pragma HLS array_partition variable=v2275 cyclic factor=2 dim=1


  #pragma HLS interface m_axi offset=slave port=v2274 bundle=axi_0

  float v2518[1000] = {(float)-0.029348, (float)-0.003449, (float)0.038818, (float)-0.023214, (float)-0.024450, (float)0.033870, (float)0.030026, (float)0.016490, (float)0.008442, (float)-0.042368, (float)0.004686, (float)-0.025045, (float)-0.010783, (float)-0.040608, (float)0.006596, (float)-0.014482, (float)0.033950, (float)0.028098, (float)-0.019142, (float)0.021964, (float)0.012657, (float)-0.002024, (float)0.037451, (float)-0.009339, (float)-0.042355, (float)-0.031412, (float)0.009600, (float)0.016172, (float)-0.016100, (float)0.010588, (float)0.042823, (float)0.025289, (float)0.024653, (float)-0.014462, (float)-0.031503, (float)0.022190, (float)-0.021064, (float)-0.028624, (float)-0.015309, (float)0.022964, (float)0.012375, (float)-0.018569, (float)0.040212, (float)-0.022723, (float)0.023444, (float)0.010156, (float)-0.016837, (float)-0.015525, (float)0.018249, (float)0.040583, (float)-0.010172, (float)0.014322, (float)-0.036880, (float)-0.011265, (float)0.033955, (float)0.001309, (float)0.037462, (float)-0.001946, (float)-0.000849, (float)-0.036519, (float)-0.010572, (float)-0.015299, (float)0.024929, (float)-0.006271, (float)0.013872, (float)0.022239, (float)0.019904, (float)-0.042664, (float)-0.002040, (float)0.030989, (float)-0.022173, (float)0.007114, (float)0.007060, (float)-0.001029, (float)-0.019358, (float)-0.014038, (float)0.029848, (float)0.016664, (float)-0.040140, (float)0.010758, (float)-0.044013, (float)-0.010207, (float)0.013315, (float)0.034463, (float)0.007437, (float)-0.025162, (float)-0.008556, (float)-0.010974, (float)0.027489, (float)-0.005185, (float)0.006434, (float)0.016270, (float)0.028565, (float)0.017937, (float)-0.030134, (float)-0.040478, (float)-0.037444, (float)-0.009081, (float)-0.025688, (float)0.022735, (float)-0.011272, (float)0.021977, (float)-0.026695, (float)-0.005175, (float)-0.007285, (float)-0.007647, (float)0.029531, (float)0.015139, (float)-0.031004, (float)-0.023970, (float)0.005867, (float)-0.020902, (float)-0.037569, (float)0.017643, (float)-0.002708, (float)-0.012846, (float)-0.006635, (float)-0.018180, (float)-0.000704, (float)0.022877, (float)-0.010944, (float)-0.024433, (float)-0.043926, (float)0.007246, (float)-0.030662, (float)0.028389, (float)0.016844, (float)0.007804, (float)-0.021768, (float)0.030383, (float)-0.006823, (float)0.035470, (float)-0.026482, (float)-0.032767, (float)-0.005363, (float)-0.034641, (float)0.005096, (float)0.023246, (float)-0.009122, (float)-0.043564, (float)-0.042246, (float)-0.031902, (float)0.036199, (float)0.004855, (float)-0.000207, (float)-0.022784, (float)-0.013968, (float)0.025974, (float)0.036943, (float)-0.001271, (float)0.034738, (float)-0.034784, (float)-0.042262, (float)0.001941, (float)0.021814, (float)0.004994, (float)0.009354, (float)-0.019699, (float)0.017696, (float)0.016552, (float)0.032452, (float)0.007044, (float)-0.013422, (float)-0.015635, (float)0.040075, (float)0.005714, (float)-0.023750, (float)0.036306, (float)0.015438, (float)-0.018065, (float)-0.031810, (float)0.010576, (float)0.042840, (float)0.026637, (float)-0.010546, (float)0.013967, (float)0.018944, (float)-0.016432, (float)0.018409, (float)-0.002139, (float)-0.017584, (float)-0.033760, (float)-0.019591, (float)0.007385, (float)0.030400, (float)-0.011085, (float)-0.014645, (float)0.010160, (float)-0.016982, (float)0.004761, (float)0.037691, (float)-0.006806, (float)-0.003674, (float)-0.038666, (float)0.020832, (float)-0.040540, (float)-0.033797, (float)-0.019465, (float)-0.042076, (float)0.006891, (float)0.017277, (float)-0.008062, (float)-0.004263, (float)0.009577, (float)0.007070, (float)0.039230, (float)0.016023, (float)-0.029423, (float)0.012827, (float)-0.027429, (float)0.041786, (float)0.041229, (float)0.017228, (float)0.008454, (float)0.003207, (float)-0.015487, (float)0.017034, (float)-0.032427, (float)-0.008781, (float)0.017798, (float)-0.016423, (float)0.031544, (float)-0.029568, (float)-0.011936, (float)-0.024749, (float)0.041273, (float)-0.014955, (float)-0.024029, (float)0.034062, (float)-0.009076, (float)-0.030926, (float)-0.035123, (float)-0.006219, (float)-0.040060, (float)0.002530, (float)0.019563, (float)-0.014586, (float)-0.036942, (float)0.015003, (float)0.033821, (float)-0.004264, (float)0.035528, (float)0.033816, (float)0.008062, (float)-0.030310, (float)-0.027021, (float)-0.021431, (float)0.017792, (float)0.028977, (float)-0.043094, (float)0.029344, (float)-0.040109, (float)0.036070, (float)0.011817, (float)0.006224, (float)0.025455, (float)0.014694, (float)0.016634, (float)0.011549, (float)0.028037, (float)0.014618, (float)0.038958, (float)-0.011223, (float)-0.029822, (float)-0.027929, (float)0.012423, (float)-0.015111, (float)-0.043920, (float)0.033058, (float)0.031794, (float)-0.014412, (float)-0.001927, (float)-0.040718, (float)-0.017474, (float)-0.035455, (float)-0.032651, (float)-0.018373, (float)-0.012068, (float)0.012486, (float)-0.007882, (float)0.005403, (float)-0.021729, (float)-0.032329, (float)-0.029539, (float)0.013523, (float)0.018025, (float)-0.017185, (float)0.030900, (float)-0.019031, (float)-0.028502, (float)-0.031182, (float)0.010542, (float)0.035817, (float)-0.007528, (float)0.023670, (float)-0.000856, (float)0.032442, (float)-0.024012, (float)-0.016240, (float)0.004775, (float)-0.042298, (float)0.013579, (float)-0.025722, (float)-0.000974, (float)-0.032662, (float)0.023040, (float)0.027159, (float)0.003122, (float)0.029876, (float)-0.034037, (float)0.038540, (float)0.018273, (float)-0.040005, (float)-0.028076, (float)-0.039239, (float)0.015617, (float)-0.026814, (float)0.024269, (float)0.009952, (float)0.037162, (float)0.028031, (float)-0.017312, (float)0.008209, (float)0.007676, (float)0.034491, (float)-0.010572, (float)-0.008537, (float)0.019189, (float)-0.007227, (float)-0.010487, (float)0.003946, (float)0.023468, (float)0.035462, (float)0.021740, (float)-0.022962, (float)-0.033740, (float)-0.038290, (float)-0.005956, (float)0.018882, (float)-0.000446, (float)-0.012126, (float)0.034043, (float)0.033999, (float)0.007681, (float)0.040391, (float)0.034709, (float)0.018121, (float)-0.025392, (float)-0.025940, (float)-0.005969, (float)-0.005077, (float)-0.015788, (float)0.029799, (float)-0.041372, (float)-0.005600, (float)-0.011793, (float)-0.004008, (float)0.039962, (float)-0.023217, (float)0.024136, (float)0.018936, (float)-0.017235, (float)-0.028170, (float)0.009704, (float)0.005441, (float)0.001090, (float)-0.043403, (float)0.029899, (float)0.031661, (float)-0.005278, (float)-0.015591, (float)0.036711, (float)0.013609, (float)0.035942, (float)0.035981, (float)-0.017475, (float)0.030315, (float)0.013974, (float)-0.003771, (float)-0.008047, (float)0.028771, (float)-0.037521, (float)-0.009948, (float)0.035111, (float)-0.007177, (float)-0.019002, (float)0.013889, (float)0.031784, (float)0.043176, (float)0.020757, (float)-0.020716, (float)0.016042, (float)-0.014301, (float)-0.025643, (float)0.012356, (float)-0.002062, (float)0.043696, (float)-0.008708, (float)-0.011586, (float)0.040202, (float)-0.017200, (float)0.011102, (float)0.021419, (float)-0.031858, (float)0.043846, (float)-0.039294, (float)-0.040705, (float)-0.005160, (float)0.036385, (float)-0.010485, (float)0.011654, (float)0.030832, (float)-0.009376, (float)-0.031707, (float)-0.037918, (float)-0.005901, (float)0.041098, (float)0.002129, (float)0.041121, (float)-0.022939, (float)0.028293, (float)-0.043906, (float)0.042640, (float)-0.040576, (float)-0.000274, (float)-0.041633, (float)0.039859, (float)-0.005507, (float)0.032794, (float)-0.041249, (float)0.019123, (float)0.042232, (float)-0.006849, (float)-0.010987, (float)-0.000767, (float)0.039246, (float)0.025690, (float)-0.026075, (float)-0.010690, (float)-0.040940, (float)-0.000561, (float)-0.000598, (float)-0.000476, (float)-0.021709, (float)-0.029822, (float)0.017520, (float)-0.019011, (float)-0.024763, (float)-0.008866, (float)0.023913, (float)-0.040469, (float)-0.036893, (float)-0.015922, (float)0.023135, (float)-0.043465, (float)-0.011037, (float)0.027487, (float)-0.028391, (float)-0.020203, (float)-0.030354, (float)0.003845, (float)0.040419, (float)-0.043983, (float)-0.032416, (float)0.019061, (float)-0.029777, (float)-0.003224, (float)0.030436, (float)0.041057, (float)-0.014719, (float)0.012132, (float)0.018559, (float)-0.021309, (float)0.008410, (float)0.024441, (float)0.037060, (float)-0.009907, (float)-0.026014, (float)0.001872, (float)0.008402, (float)-0.028527, (float)0.028320, (float)-0.020465, (float)0.018504, (float)-0.000577, (float)0.003185, (float)-0.033369, (float)-0.024419, (float)0.004250, (float)0.015277, (float)0.017681, (float)-0.013454, (float)-0.033187, (float)0.004573, (float)0.033152, (float)0.005151, (float)-0.007923, (float)0.031891, (float)-0.008680, (float)-0.009816, (float)-0.037619, (float)-0.029966, (float)0.002580, (float)0.021156, (float)-0.020379, (float)0.026964, (float)-0.003445, (float)-0.020894, (float)-0.038904, (float)0.017256, (float)-0.010612, (float)0.012248, (float)0.005053, (float)0.007918, (float)0.043781, (float)-0.026343, (float)0.042103, (float)0.041500, (float)-0.032146, (float)0.040063, (float)-0.039243, (float)-0.029358, (float)-0.007525, (float)0.040605, (float)-0.031979, (float)0.042712, (float)0.014826, (float)-0.040689, (float)0.022932, (float)-0.022217, (float)-0.015751, (float)0.039718, (float)-0.005767, (float)0.029804, (float)-0.041944, (float)-0.040422, (float)-0.007687, (float)-0.029141, (float)0.009824, (float)0.010235, (float)0.039503, (float)-0.004385, (float)0.022672, (float)0.010032, (float)0.011654, (float)0.023831, (float)-0.040395, (float)0.015248, (float)0.028203, (float)-0.040579, (float)0.002156, (float)0.029465, (float)0.021393, (float)0.020309, (float)0.007247, (float)-0.036013, (float)-0.043385, (float)0.035568, (float)0.018792, (float)-0.018445, (float)0.037378, (float)0.002011, (float)-0.032238, (float)0.027422, (float)0.022990, (float)0.003047, (float)0.003583, (float)-0.012336, (float)-0.043858, (float)-0.013174, (float)-0.024691, (float)-0.032455, (float)0.005750, (float)0.040116, (float)-0.017327, (float)0.012502, (float)0.025134, (float)0.043900, (float)-0.017382, (float)-0.020642, (float)-0.039811, (float)-0.013919, (float)-0.007789, (float)0.025364, (float)-0.004735, (float)-0.021458, (float)0.027365, (float)-0.018716, (float)-0.030522, (float)-0.031355, (float)-0.019808, (float)0.010291, (float)0.017157, (float)0.009058, (float)-0.035742, (float)-0.036043, (float)-0.014214, (float)-0.006015, (float)-0.003191, (float)-0.040522, (float)-0.035133, (float)-0.012167, (float)-0.014711, (float)0.027724, (float)-0.011123, (float)-0.004765, (float)-0.021483, (float)0.003812, (float)-0.032540, (float)0.039738, (float)-0.010697, (float)-0.028816, (float)0.014846, (float)-0.024194, (float)0.038781, (float)-0.028918, (float)0.008988, (float)-0.027663, (float)-0.029049, (float)0.006922, (float)0.006406, (float)0.035312, (float)0.027669, (float)0.021717, (float)-0.022998, (float)0.042842, (float)-0.014449, (float)0.025130, (float)0.027587, (float)-0.040361, (float)0.024461, (float)-0.043804, (float)0.000144, (float)-0.030686, (float)-0.030566, (float)0.027650, (float)-0.009578, (float)0.023559, (float)0.025138, (float)-0.008723, (float)0.038478, (float)0.015995, (float)-0.017621, (float)-0.018347, (float)-0.001432, (float)0.024961, (float)0.031521, (float)-0.004198, (float)0.015050, (float)-0.020280, (float)-0.028072, (float)-0.023390, (float)0.035396, (float)-0.012517, (float)0.023776, (float)-0.001520, (float)-0.005521, (float)0.006548, (float)-0.015459, (float)0.037654, (float)0.042402, (float)0.000050, (float)-0.026653, (float)0.023576, (float)0.017296, (float)-0.006425, (float)0.000068, (float)0.031336, (float)0.021221, (float)0.020394, (float)-0.001726, (float)-0.036497, (float)0.027017, (float)-0.005753, (float)-0.033481, (float)-0.038529, (float)-0.037550, (float)0.010093, (float)-0.022489, (float)0.026452, (float)0.043129, (float)0.020593, (float)-0.004032, (float)-0.033693, (float)-0.011198, (float)-0.018653, (float)-0.022121, (float)0.002313, (float)-0.043113, (float)-0.038979, (float)0.015472, (float)0.038184, (float)0.010428, (float)0.016288, (float)0.027532, (float)0.032793, (float)0.022655, (float)-0.013782, (float)-0.040050, (float)0.014768, (float)-0.006688, (float)0.037016, (float)-0.013848, (float)-0.015298, (float)0.035064, (float)-0.042970, (float)0.041441, (float)0.001494, (float)-0.003628, (float)0.019817, (float)-0.008204, (float)0.038840, (float)0.009228, (float)-0.002125, (float)0.040877, (float)0.001577, (float)-0.024948, (float)-0.026094, (float)0.037644, (float)-0.025043, (float)0.042562, (float)-0.039482, (float)0.008319, (float)0.038643, (float)-0.013195, (float)-0.002363, (float)-0.043641, (float)0.014967, (float)0.016509, (float)0.008983, (float)0.016803, (float)0.029171, (float)0.019961, (float)0.037827, (float)0.008929, (float)0.012504, (float)0.021273, (float)-0.012088, (float)-0.029529, (float)0.029221, (float)0.018129, (float)0.033315, (float)-0.014239, (float)-0.004023, (float)0.039165, (float)-0.035859, (float)0.022726, (float)0.034363, (float)-0.021814, (float)-0.020168, (float)-0.034537, (float)0.014882, (float)0.004347, (float)0.015437, (float)-0.002150, (float)-0.026996, (float)-0.005407, (float)0.017497, (float)0.043241, (float)-0.032579, (float)-0.020893, (float)-0.015830, (float)0.012286, (float)-0.035890, (float)-0.018940, (float)0.034107, (float)-0.007586, (float)0.030287, (float)-0.024649, (float)-0.038024, (float)-0.041743, (float)0.001301, (float)-0.038142, (float)-0.044191, (float)0.038936, (float)0.019872, (float)0.014978, (float)-0.032157, (float)-0.023024, (float)-0.010230, (float)-0.015784, (float)-0.019936, (float)-0.013432, (float)-0.039870, (float)0.027784, (float)0.043579, (float)-0.036313, (float)0.029965, (float)-0.022144, (float)-0.029000, (float)-0.001717, (float)-0.019362, (float)-0.015116, (float)0.006085, (float)0.008628, (float)-0.028015, (float)-0.043357, (float)0.016253, (float)0.034184, (float)0.031812, (float)-0.006335, (float)-0.012484, (float)-0.026792, (float)-0.042422, (float)-0.006232, (float)-0.022147, (float)-0.043971, (float)-0.003008, (float)0.005609, (float)-0.012491, (float)0.015378, (float)0.026912, (float)0.007981, (float)0.020274, (float)0.026912, (float)-0.010283, (float)0.028826, (float)0.001631, (float)-0.001615, (float)-0.041671, (float)-0.019858, (float)0.032005, (float)0.028963, (float)0.037139, (float)0.013887, (float)0.032358, (float)-0.036362, (float)0.004536, (float)-0.029205, (float)0.033180, (float)0.030866, (float)-0.043218, (float)-0.027196, (float)0.028791, (float)-0.037054, (float)-0.005660, (float)-0.029311, (float)0.005866, (float)-0.007224, (float)-0.017264, (float)0.039681, (float)-0.025084, (float)-0.008125, (float)-0.005874, (float)0.000177, (float)0.021050, (float)-0.005724, (float)0.030527, (float)-0.025369, (float)-0.008583, (float)-0.019112, (float)0.012754, (float)0.023292, (float)0.032683, (float)0.003234, (float)-0.005810, (float)-0.035412, (float)0.016710, (float)0.032177, (float)-0.017465, (float)-0.041760, (float)-0.019328, (float)-0.010874, (float)-0.006704, (float)-0.037554, (float)-0.037750, (float)0.036119, (float)0.030071, (float)-0.018792, (float)-0.035165, (float)-0.008129, (float)-0.041581, (float)0.023220, (float)0.032753, (float)-0.029626, (float)-0.025873, (float)0.037983, (float)0.017267, (float)0.013232, (float)-0.021531, (float)-0.004744, (float)0.013695, (float)-0.013446, (float)0.022747, (float)0.035969, (float)-0.032217, (float)-0.032703, (float)-0.035066, (float)0.000793, (float)0.024012, (float)0.035252, (float)-0.032355, (float)0.023556, (float)-0.038937, (float)0.041738, (float)0.034189, (float)-0.009728, (float)-0.023148, (float)0.019205, (float)0.026567, (float)0.002376, (float)-0.041562, (float)-0.037487, (float)-0.043845, (float)-0.000156, (float)-0.003242, (float)0.032757, (float)0.029425, (float)-0.035956, (float)-0.036849, (float)-0.035404, (float)-0.016415, (float)0.010587, (float)-0.042301, (float)-0.026188, (float)0.035289, (float)0.011247, (float)0.011255, (float)-0.005988, (float)0.002300, (float)-0.036330, (float)0.018251, (float)0.043518, (float)-0.025121, (float)-0.034114, (float)0.014548, (float)0.001215, (float)0.004641, (float)0.004203, (float)0.019236, (float)-0.039935, (float)0.005256, (float)-0.039760, (float)0.011686, (float)0.029957, (float)-0.040420, (float)-0.034891, (float)-0.016331, (float)0.007730, (float)0.015973, (float)-0.017213, (float)-0.025180, (float)-0.002026, (float)-0.027495, (float)-0.002396, (float)-0.044001, (float)0.037856, (float)0.023124, (float)0.007883, (float)0.005791, (float)-0.003217, (float)0.026799, (float)0.023124, (float)0.042391, (float)0.003983, (float)-0.038931, (float)-0.015293, (float)-0.035319, (float)0.014093, (float)-0.027154, (float)0.039554, (float)-0.002232, (float)-0.029194, (float)-0.010282, (float)0.010311, (float)0.007989, (float)0.031681, (float)-0.041311, (float)0.044056, (float)-0.038351, (float)-0.009725, (float)-0.018014, (float)0.000384, (float)0.030019, (float)0.003248, (float)-0.026441, (float)0.029351, (float)-0.008693, (float)-0.039748, (float)-0.001312, (float)0.023890, (float)-0.006211, (float)-0.025106, (float)0.028736, (float)-0.012345, (float)-0.012651, (float)-0.011584, (float)0.019119, (float)0.034259, (float)0.038419, (float)0.033381, (float)-0.030883, (float)-0.020787, (float)0.002574, (float)0.011123, (float)0.027495, (float)-0.008420, (float)-0.003677, (float)0.013448, (float)-0.001587};	// L2964
  #pragma HLS array_partition variable=v2518 cyclic factor=2 dim=1
  #pragma HLS bind_storage variable=v2518 type=ram_2p impl=lutram

  forward_node166(v2274, v2276, v2298, v2297);	// L2965
  forward_node161(v2299, v2301, v2300);	// L2966
  forward_node156(v2302, v2304, v2303);	// L2967
  forward_node149(v2305, v2277, v2307, v2313, v2312);	// L2968
  forward_node140(v2278, v2314, v2306, v2308, v2319, v2315, v2318);	// L2969
  forward_node133(v2279, v2316, v2309, v2321, v2320);	// L2970
  forward_node125(v2280, v2317, v2322, v2311, v2323, v2310);	// L2971
  forward_node113(v2326, v2324, v2281, v2335, v2334, v2333, v2332);	// L2972
  forward_node106(v2282, v2327, v2336, v2338, v2337);	// L2973
  forward_node97(v2328, v2283, v2339, v2325, v2344, v2340, v2343);	// L2974
  forward_node90(v2284, v2329, v2341, v2346, v2345);	// L2975
  forward_node82(v2342, v2347, v2285, v2331, v2330, v2348);	// L2976
  forward_node75(v2286, v2351, v2349, v2358, v2357);	// L2977
  forward_node68(v2359, v2287, v2352, v2361, v2360);	// L2978
  forward_node59(v2288, v2353, v2350, v2362, v2367, v2366, v2363);	// L2979
  forward_node52(v2289, v2354, v2364, v2369, v2368);	// L2980
  forward_node44(v2370, v2290, v2365, v2356, v2371, v2355);	// L2981
  forward_node37(v2372, v2374, v2291, v2381, v2380);	// L2982
  forward_node30(v2292, v2375, v2382, v2384, v2383);	// L2983
  forward_node21(v2373, v2376, v2385, v2293, v2390, v2389, v2386);	// L2984
  forward_node14(v2377, v2294, v2387, v2392, v2391);	// L2985
  forward_node6(v2295, v2393, v2388, v2379, v2394, v2378);	// L2986
  float v2519[512];	// L2987
  #pragma HLS array_partition variable=v2519 cyclic factor=2 dim=1
  #pragma HLS bind_storage variable=v2519 type=ram_2p impl=lutram

  forward_node3(v2395, v2519);	// L2988
  forward_node0(v2519, v2296, v2518, v2275);	// L2989
}

