'
' From an XLS spreadsheet at http://www.itu.int/oth/R0A0400005F/en
'
' This macros calculates the clear-air portion of
' Recommendation ITU-R P.452-14 (2009) Annex 1 §1 through §4 (with the
' exception of §4.6 on "Additional clutter losses") and Appendix 2
'
'
' The spreadsheet contains this Displaimer:
'
'     This software is provided "as is" without any warranty of any
'     kind, either expressed, implied, or statutory, including, but
'     not limited to, any warranty that the software will conform to
'     specifications, any implied warranties of merchantability,
'     fitness for a particular purpose, or freedom from infringement,
'     any warranty that the software will be error free, or any
'     warranty that documentation, if provided, will conform to the
'     software. In no event shall the U.S. Government, or the U.S.
'     Government's contractors or subcontractors, be liable for any
'     damages, including, but not limited to, direct, indirect,
'     special or consequential damages, arising out of, resulting
'     from, or in any way connected with this software, whether or not
'     based upon warranty, contract, tort, or otherwise, whether or
'     not injury was sustained by persons or property or otherwise,
'     and whether or not loss was sustained from, or arose out of the
'     results of, or use of, the software or services provided
'     hereunder.
'
'     Recipient agrees to waive any and all claims against the U.S.
'     Government, and the U.S. Government's contractors and
'     subcontractors, and shall indemnify and hold harmless the U.S.
'     Government, and the U.S. Government's contractors and
'     subcontractors, for any damage that recipient may incur from
'     recipient's prior or future use of the provided software,
'     including any damages from products based on, or resulting from,
'     the use thereof.
'
' And later:
'
'     Macros performing individual calculations (e.g., diffraction,
'     line-of-sight propagation, anomalous propagation, etc.) may be
'     copied and run indpendently in other spreadsheets. ...
'


Rem Attribute VBA_ModuleType=VBAModule
Option VBASupport 1
Option Base 1
Function Path_Profile_Analysis(f, DELTA_N, htg, hrg, param)
	Application.Volatile
	' Function used for path profile analysis

	' Reference: ITU-R P.452-14 Appendix 2 to Annex 1

	' Inputs:
	' f = frequency (GHz)
	' DELTA_N =atmosphere refractivity gradient through lowest 1km (N-units/km)
	' D = Nx1 vector of great-circle distances along the path where di is the
	'     distance (km) of the ith terrain point from the TX station (i=1
	'     corresponds to the TX station and i=n to the RX station)
	' H = Nx1 vector of terrain heights where hi is the height of the ith
	'     terrain point (meters amsl). H(1) is the terrain height at the TX
	'     station and H(n) is the terrain height at the RX station
	' htg = antenna center height (above ground level) at TX (m)
	' hrg = antenna center height (above ground level) at RX (m)

	' param = desired output parameter where
	'       param = 1: ae = the effective earth radius (km)
	'       param = 2: dtot = the total great circle path distance between the terminals(km)
	'       param = 3: hts = TX ant height amsl (m)
	'       param = 4: hrs = RX ant height amsl (m)
	'       param = 5: theta_t = horizon elev angle (above local horizontal)at TX ant(mrad)
	'       param = 6; theta_r = horizon elev angle (above local horizontal)at RX ant (mrad)
	'       param = 7: theta_out = path angular distance (mrad)
	'       param = 8: PATH_TYPE = 3 for transhorizon paths; PATH_TYPE =1 for Line-of-Sight
	'       param = 9: hst = height of the smooth-earth surface (amsl) at TX station (m)
	'       param = 10: hsr = height of the smooth-earth surface (amsl) at RX station (m)
	'       param = 11: hm = terrain roughness parameter (m)
	'       param = 12: hte = effective height of TX ant (m)
	'       param = 13: hre = effective height of RX ant (m)
	'       param = 14: dlt = great circle distance from TX ant to its horizon point (km)
	'       param = 15: dlr = great circle distance from RX ant to its horizon point (km)

	' speed of light in vacuum (m/s)
	c = 299800000#

	'wavelength (m)
	lambda = c / (f * 1000000000#)

	' Calculate k50 from Equation (5) and ae from Equation (6a)
	k50 = median_eff_Re(DELTA_N, 1)
	ae = median_eff_Re(DELTA_N, 2)

	' Get the Distance and Terrain Height vectors from the Step_4 worksheet
	n = Sheets("Step_4").Range("e50")
	Rnge1$ = "A2:A" + Mid$(Str$(n + 1), 2)
	Rnge2$ = "B2:B" + Mid$(Str$(n + 1), 2)
	D = Sheets("Step_4").Range(Rnge1) ' distance (km)
	H = Sheets("Step_4").Range(Rnge2) ' height (m)

	hgt = H(1, 1)   ' ground height (amsl) at TX station (m)
	hgr = H(n, 1)   ' ground height (amsl) at RX station (m)
	dtot = D(n, 1)  ' total distance (km)
	hts = htg + hgt ' transmitter height sum (sender antenna height + sender hight above ground)
	hrs = hrg + hgr ' receiver height sum
	' Equation (143)
	theta_td = (hrs - hts) / dtot - 1000 * dtot / (2 * ae)

	If (n > 2) Then ' There is at least one intervening terrain point between terminals
		' This creates vectors D1 and h1 which are just the vectors D(2,3,...,n-1) and
		' h(2,3,...,n-1).  There is no first and last point.
		Rnge1$ = "A3:A" + Mid$(Str$(n), 2)
		Rnge2$ = "B3:B" + Mid$(Str$(n), 2)
		D1 = Sheets("TEST_PROFILE").Range(Rnge1)
		h1 = Sheets("TEST_PROFILE").Range(Rnge2)

		' Because it is unknown how many points the user will enter on the terrain profile,
		' and array sizes must be defined with constants, the THETA array is defined with
		' maximum possible size though most of it probably won't be used.
		Dim theta(65535) As Double
		theta_max = -9000000000#
		' Section 4 of Appendix 2 of Annex 1
		' Find the maximum value in the THETA array - Equations (141) and (142)
		For counter = 1 To n - 2
			theta(counter) = (h1(counter, 1) - hts) / D1(counter, 1) - 1000 * D1(counter, 1) / (2 * ae)
			If theta(counter) > theta_max Then
				theta_max = theta(counter)
			End If
		Next counter

		' Given by Equation (140)
		If (theta_max > theta_td) Then ' path is trans-horizon
			PATH_TYPE = 3
		Else
			PATH_TYPE = 1
		End If

		If PATH_TYPE = 3 Then
			' Identify the Interfering antenna horizon elevation angle - Section 5.1.1
			theta_t = theta_max ' Equation (144)
			' Find the index of the first point, x, where THETA(x) = theta_max.  This will
			' determine the Interfering antenna horizon distance, dlt - Section 5.1.2
			flag = 0
			For counter = 1 To n - 1
				If (theta(counter) = theta_max) And (flag = 0) Then
					dlt = D(counter + 1, 1) ' Equation (145)
					index_t = counter + 1
					flag = 1
				End If
			Next counter
			' Find the Interfered-with antenna horizon elevation angle, theta_r - Section 5.1.3
			Dim theta2(65535) As Double
			theta_r = -9000000000#
			For counter = 1 To n - 2
				' Equation (147)
				theta2(counter) = (h1(counter, 1) - hrs) / (dtot - D1(counter, 1)) - 1000 * (dtot - D1(counter, 1)) / (2 * ae)
				' This If in the For/Next loop effectively is Equation (146)
				If theta2(counter) > theta_r Then
					theta_r = theta2(counter)
					' Use the index of theta_r to find interfered-with antenna horizon distance, dlr - Section 5.1.4
					dlr = dtot - D(counter + 1, 1) ' Equation (148)
					index_r = counter + 1
				End If
			Next counter
			theta_out = 1000 * dtot / ae + theta_t + theta_r  ' Equation (149)
		Else   ' The path is LoS
			' Since the path is LoS, use the alternate definitions from Table 3 for these 4 parameters
			' Note 1: The text in Table 3 is inaccurate.  It should say, "For a line-of
			'   sight path, each is set to the elevation angle TO the other terminal."
			' Note 2:  The equation for the elevation angle from the interfering transmitter to the victim
			'   receiver is derived as theta_td in equation (143) but not as theta_t even though this is
			'   the required parameter.  Likewise, the equation for elevation angle from the victim receiver
			'   to the interfering transmitter, theta_r, is not provided in Rec P.452-14.  However, it is
			'   similar to that of theta_t.  It is confirmed in equation (71) of Rec P.1812-1.
			theta_t = ((hrs - hts) / dtot) - ((1000 * dtot) / (2 * ae))   ' Taken from equation (143)
			theta_r = ((hts - hrs) / dtot) - ((1000 * dtot) / (2 * ae))   ' Taken from equation (71) of P.1812-1
			' Note: Equation (149) is listed under that Trans-Horizon path method, but applies to LoS as well
			theta_out = 1000 * dtot / ae + theta_t + theta_r

			' The diff_loss function is called to find the distance to the primary edge
			' The dummy values of 1 are inserted in several places as they are not needed for this calculation.
			i_m50 = diff_loss(1, f, DELTA_N, hrs, hts, 1, 1, 1, 4)
			dlt = D(i_m50, 1)
			dlr = dtot - dlt
			index_t = i_m50
			index_r = i_m50
		End If

		' Derive the smooth-Earth surface - Section 5.1.6.3
		sumV1 = 0
		Dim Di(65535) As Double
		Dim Di_1(65535) As Double
		Dim Hi(65535) As Double
		Dim Hi_1(65535) As Double
		Dim V1(65535) As Double
		For counter = 1 To n - 1
			Di(counter) = D(counter + 1, 1)
			Di_1(counter) = D(counter, 1)
			Hi(counter) = H(counter + 1, 1)
			Hi_1(counter) = H(counter, 1)
			' Equation (152b)
			V1(counter) = (Di(counter) - Di_1(counter)) * (Hi(counter) + Hi_1(counter))
			sumV1 = sumV1 + V1(counter)
		Next counter
		ha = sumV1 / (2 * dtot)

		' Equation (151b)
		sumV2 = 0
		For counter = 1 To n - 1
			sumV2 = sumV2 + (3 * (Di(counter) - Di_1(counter)) * (Di(counter) + Di_1(counter) - dtot) * (Hi(counter) + Hi_1(counter) - 2 * ha) + (Di(counter) - Di_1(counter)) ^ 2 * (Hi(counter) - Hi_1(counter)))
		Next counter
		m = 1 / dtot ^ 3 * sumV2

		hst = ha - m * dtot / 2   ' Equation (153)
		hsr = hst + m * dtot   ' Equation (154)
		hst = correct_smooth_earth(dtot, hst, hgt, hsr, hgr, m, 1)   ' Equation (155a)
		hsr = correct_smooth_earth(dtot, hst, hgt, hsr, hgr, m, 2)   ' Equation (155b)
		m = correct_smooth_earth(dtot, hst, hgt, hsr, hgr, m, 3)   ' Equation (156)
		hte = hts - hst   ' Defined in Table 7
		hre = hrs - hsr   ' Defined in Table 7

		' Calculate terrain roughness, hm - Section 5.1.6.4
		ilt = index_t
		ilr = index_r

		' This section effectively is Equation (157)
		Dim hs(65535) As Double
		Dim Z(65535) As Double
		For counter = 1 To n
			hs(counter) = hst + m * D(counter, 1)
			Z(counter) = H(counter, 1) - hs(counter)
		Next counter
		hm = -9000000000#
		For counter = ilt To ilr
			If Z(counter) > hm Then
				hm = Z(counter)
			End If
		Next counter

		Dim OUTVEC3(15) As Double
		OUTVEC3(1) = ae
		OUTVEC3(2) = dtot
		OUTVEC3(3) = hts
		OUTVEC3(4) = hrs
		OUTVEC3(5) = theta_t
		OUTVEC3(6) = theta_r
		OUTVEC3(7) = theta_out
		OUTVEC3(9) = hst
		OUTVEC3(10) = hsr
		OUTVEC3(11) = hm
		OUTVEC3(12) = hte
		OUTVEC3(13) = hre
		OUTVEC3(14) = dlt
		OUTVEC3(15) = dlr
		OUTVEC3(8) = PATH_TYPE
		Path_Profile_Analysis = OUTVEC3(param)
	End If
End Function




Function parameter_betao(theta, dtm, dlm)

	' BETAo = parameter_betao(theta, dtm, dlm)
	' Function to compute parameter BETAo% (the % time that the refractivity
	' gradient (DELTA-N) exceeds 100 N-units/km in the first 100m of the lower
	' atmosphere.

	' Reference: ITU-R P.452-14 Section 3.2.1

	' First define the 3 types of radio-climate zones:
	' 1) COASTAL LAND and shore areas (code A1) is land adjacent to the sea up
	' to an altitude of 100 m (above mean sea level or amsl),but limited to a
	' distance of 50 km from the nearest sea area
	'
	' 2)INLAND (code A2) is ALL land, other than coastal land defined in (1)
	'
	' 3) SEA (code B) is all seas, oceans, and other large bodies of water
	' covering a circle of at least 100km diameter
	'
	' The parameter BETAo depends on the path center location as well as how
	' much of the interference path is over land and how much over water

	' Inputs:
	' theta = the path center latitude (deg)
	' dtm = the longest continuous land (inland + coastal land) section of the
	'       great circle interference path (km)
	' dlm = the longest continuous inland section of the great circle path(km)

	' Output:
	' BETAo (%) = %time that DELTA-N is expected to exceed 100 in first 100m of
	' atmosphere


	' Equation (3a)
	tau = 1 - Exp(-(0.000412 * dlm ^ 2.41))
	' Equation (3)
	mu1 = (10 ^ (-dtm / (16 - 6.6 * tau)) + (10 ^ -(0.496 + 0.354 * tau)) ^ 5) ^ 0.2

	If (mu1 > 1) Then
		mu1 = 1
	End If

	' Equations (2) and (4)
	If (Abs(theta) <= 70) Then
		mu4 = 10 ^ ((-0.935 + 0.0176 * Abs(theta)) * Log(mu1) / Log(10))
		BETAo = 10 ^ (-0.015 * Abs(theta) + 1.67) * mu1 * mu4
	Else
		mu4 = 10 ^ (0.3 * Log(mu1) / Log(10))
		BETAo = 4.17 * mu1 * mu4
	End If
	parameter_betao = BETAo

End Function

Function median_eff_Re(DELTA_N, param)

	' Function to compute median effective Earth radius factor

	' Reference: ITU-R P.452-14 Section 3.2.1

	' Inputs:
	' DELTA_N = the refractivity gradient (a positive quantity) in the first
	' 1km of the atmosphere (i.e. mean change in refractivity from ground level
	' to 1 km altitude where refractivity is defined to be N = (n-1)*10^6 where
	' n is the refractive index. DELTA_N is in units of "N-units/km").
	' param selects if output is k50 (param=1) or ae (param=2)

	' Outputs:
	' k50 = the median eff Earth radius factor (dimensionless)
	' ae = the median effective Earth radius (km)

	' Equation (5)
	k50 = 157 / (157 - DELTA_N)
	' Equation (6a)
	' 6371 km is assumed to be true earth radius
	ae = 6371# * k50

	If param = 1 Then
		median_eff_Re = k50
	Else
		median_eff_Re = ae
	End If

End Function






Rem Attribute VBA_ModuleType=VBAModule
Option VBASupport 1
Option Base 1
Function diff_loss(p, f, DELTA_N, hrs, hts, L_bfsg, L_b0p, BETAo, param)

	' This function implements section 4.2 of Recommendation P.452-14
	' Developed by Glenn Feldhake Dec 2007, updated Dec 2009 and June 2010

	' Inputs:
	'   p: Percent time for which loss is not exceeded (%)
	'   f: Frequency (GHz)
	'   hts: Transmitting antenna centre height above mean sea level (m)
	'   hrs: Receiving antenna centre height above mean sea level (m)
	'   L_bfsg: Basic tx LoS loss due to free space propagation and gass (dB)
	'   L_b0p: Basic tx LoS loss not exceeded p% of time (dB)
	'   BETAo:  The time percentage for which refractive index lapse-rates exceeding
	'       100 N-units/km can be expected in the first 100 m of lower atmosphere.
	'   param: Output selection
	'       1 = Diffraction loss not exceeded p% of time, L_dp;
	'       2 = Median basic tx loss due to diffraction, L_bd50;
	'       3 = Basic tx loss due to diffraction not exceeded p%, L_bd;
	'       4 = Distance from transmitter to the principle edge defined in the
	'           median diffraction loss calculation, i_m50
	' Outputs:
	'   (See input "param")

	' Get the Distance and Terrain Height vectors from the Step_4 worksheet
	n = Sheets("Step_4").Range("e50")
	Rnge1$ = "A2:A" + Mid$(Str$(n + 1), 2)
	Rnge2$ = "B2:B" + Mid$(Str$(n + 1), 2)
	D = Sheets("Step_4").Range(Rnge1)
	H = Sheets("Step_4").Range(Rnge2)
	dtot = D(n, 1)

	' Calculate wavelength, median effective Earth radius and path length
	lambda = 0.3 / f
	ae = median_eff_Re(DELTA_N, 2)

	flag = 0
	' Section 4.2.1 of Rec P.452-14 (Calculation of Median Diffraction Loss)
	' Calculate the median diffraction loss for the principal edge
	' Equation (14)
	zeta_m = Cos(Atn(0.001 * (hrs - hts) / dtot))

	' The following is effectively Equation (15)
	nu_m50 = -9000000000#
	For counter = 2 To n - 1
		d_i = D(counter, 1)
		' Equation (15a)
		H_i = H(counter, 1) + (10 ^ 3) * ((d_i * (dtot - d_i)) / (2 * ae)) - (hts * (dtot - d_i) + hrs * d_i) / dtot
		If nu_m50 < zeta_m * H_i * Sqr((0.002 * dtot) / (lambda * d_i * (dtot - d_i))) Then
			nu_m50 = zeta_m * H_i * Sqr((0.002 * dtot) / (lambda * d_i * (dtot - d_i)))
			i_m50 = counter
		End If
	Next counter

	If param = 4 Then
		diff_loss = i_m50
		flag = 1
	Else
		' Equation (16)
		If nu_m50 < -0.78 Then
			L_m50 = 0
		Else
			L_m50 = Jnu(nu_m50)
		End If
		' And also continue with the calculations in Rec P.452-14 Sections 4.2
		If L_m50 = 0 Then
			L_d50 = 0
			L_dbeta = 0
			L_dp = 0 ' A mathmatically reduced form of Equation (32)
			L_bd50 = L_bfsg ' A mathmatically reduced form of Equation (33)  Corrected 2 June 2010
			L_bd = L_b0p ' A mathmatically reduced form of Equation (34)  Corrected 2 June 2010

			If param = 1 Then ' Output L_dp
				diff_loss = L_dp
			ElseIf param = 2 Then ' Output L_bd50
				diff_loss = L_bd50
			Else  ' Output L_bd
				diff_loss = L_bd
			End If

			flag = 1
		End If
	End If
	If flag = 0 Then   ' There is a diffraction loss component so continue calculation
		d_im50 = D(i_m50, 1)
		h_im50 = H(i_m50, 1)
		' Calculate the median diffraction loss for transmitter-side secondary edge
		If i_m50 = 2 Then
			L_t50 = 0
		ElseIf i_m50 <= 2 Then  ' (See condition under equation (19))
			L_t50 = 0
		Else
			' Equation (17)
			zeta_t = Cos(Atn(0.001 * (h_im50 - hts) / d_im50))
			' The following is effectively Equation (18)
			nu_t50 = -9000000000#
			For counter = 2 To i_m50 - 1
				d_i = D(counter, 1)
				' Equation (18a)
				H_i = H(counter, 1) + (10 ^ 3) * (d_i * (d_im50 - d_i) / (2 * ae)) - ((hts * (d_im50 - d_i) + h_im50 * d_i) / d_im50)
				If nu_t50 < zeta_t * H_i * Sqr((0.002 * d_im50) / (lambda * d_i * (d_im50 - d_i))) Then
					nu_t50 = zeta_t * H_i * Sqr((0.002 * d_im50) / (lambda * d_i * (d_im50 - d_i)))
					i_t50 = counter
				End If
			Next counter
			' Equation (19)
			If nu_t50 < -0.78 Then
				L_t50 = 0
			Else
				L_t50 = Jnu(nu_t50)
			End If
		End If

		' Calculate the median diffraction loss for receiver-side secondary edge
		If i_m50 = n - 1 Then
			L_r50 = 0
		ElseIf i_m50 >= n - 1 Then ' (See condition under equation (22))
			L_r50 = 0
		Else
			' Equation (20)
			zeta_r = Cos(Atn(0.001 * (hrs - h_im50) / (dtot - d_im50)))
			' The following is effectively Equation (21)
			nu_r50 = -9000000000#
			For counter = i_m50 + 1 To n - 1
				d_i = D(counter, 1)
				' Equation (21a)
				H_i = H(counter, 1) + (10 ^ 3) * ((d_i - d_im50) * (dtot - d_i) / (2 * ae)) - ((h_im50 * (dtot - d_i) + hrs * (d_i - d_im50)) / (dtot - d_im50))
				If nu_r50 < zeta_r * H_i * Sqr((0.002 * (dtot - d_im50)) / (lambda * (d_i - d_im50) * (dtot - d_i))) Then
					nu_r50 = zeta_r * H_i * Sqr((0.002 * (dtot - d_im50)) / (lambda * (d_i - d_im50) * (dtot - d_i)))
					i_r50 = counter
				End If
			Next counter
			' Equation (22)
			If nu_r50 < -0.78 Then
				L_r50 = 0
			Else
				L_r50 = Jnu(nu_r50)
			End If
		End If

		' Calculate the combination of the edge losses for median Earth curvature
		' Equation (23)
		If nu_m50 > -0.78 Then
			L_d50 = L_m50 + (1 - Exp(L_m50 / (-6))) * (L_t50 + L_r50 + 10 + 0.04 * dtot)
		Else
			L_d50 = 0
		End If

		' Calculate median effective Earth radius factor exceeded beta% of time
		abeta = 6371 * 3#  ' Equation (6b)
		' Calculate heights and distances of secondary edges
		If L_t50 > 0 Then
			h_it50 = H(i_t50, 1)
			d_it50 = D(i_t50, 1)
		End If
		If L_r50 > 0 Then
			h_ir50 = H(i_r50, 1)
			d_ir50 = D(i_r50, 1)
		End If

		' Calculate the prinicipal edge diffraction not exceeded beta% time
		' Equation (24a)
		H_imbeta = h_im50 + (10 ^ 3) * (d_im50 * (dtot - d_im50) / (2 * abeta)) - ((hts * (dtot - d_im50) + hrs * d_im50) / dtot)
		' Equation (24)
		nu_mbeta = zeta_m * H_imbeta * Sqr((0.002 * dtot) / (lambda * d_im50 * (dtot - d_im50)))
		' Equation (25)
		If nu_mbeta < -0.78 Then
			L_mbeta = 0
		Else
			L_mbeta = Jnu(nu_mbeta)
		End If

		' Calculate the transmitter-side secondary edge diffraction not exceeded beta% time
		If L_t50 = 0 Then
			L_tbeta = 0
		Else
			' Equation (26a)
			H_itbeta = h_it50 + (10 ^ 3) * (d_it50 * (d_im50 - d_it50) / (2 * abeta)) - ((hts * (d_im50 - d_it50) + h_im50 * d_it50) / d_im50)
			' Equation (26)
			nu_tbeta = zeta_t * H_itbeta * Sqr((0.002 * d_im50) / (lambda * d_it50 * (d_im50 - d_it50)))
			' Equation (27)
			If nu_tbeta < -0.78 Then
				L_tbeta = 0
			Else
				L_tbeta = Jnu(nu_tbeta)
			End If
		End If

		' Calculate the receiver-side secondary edge diffraction not exceeded beta% time
		If L_r50 = 0 Then
			L_rbeta = 0
		Else
			' Equation (28a)
			H_irbeta = h_ir50 + (10 ^ 3) * ((d_ir50 - d_im50) * (dtot - d_ir50) / (2 * abeta)) - ((h_im50 * (dtot - d_ir50) + hrs * (d_ir50 - d_im50)) / (dtot - d_im50))
			' Equation (28)
			nu_rbeta = zeta_r * H_irbeta * Sqr((0.002 * (dtot - d_im50)) / (lambda * (d_ir50 - d_im50) * (dtot - d_ir50)))
			' Equation (29)
			If nu_rbeta < -0.78 Then
				L_rbeta = 0
			Else
				L_rbeta = Jnu(nu_rbeta)
			End If
		End If

		' Calculate the combination of the edge losses not exceeded for beta% time
		' Equation (30)
		If nu_mbeta > -0.78 Then
			L_dbeta = L_mbeta + (1 - Exp(L_mbeta / (-6))) * (L_tbeta + L_rbeta + 10 + 0.04 * dtot)
		Else
			L_dbeta = 0
		End If

		' Section 4.2.3 The diffraction loss not exceeded p% of time
		If p = 50 Then
			F_i = 0  ' Equation (31a)
		ElseIf 50 > p And p > BETAo Then
			F_i = (inv_cum_norm(p / 100)) / (inv_cum_norm(BETAo / 100))  ' Equation (31b)
		Else
			F_i = 1  ' Equation (31c)
		End If

		If param = 1 Then ' Output L_dp as in Equation (32)
			diff_loss = L_d50 + F_i * (L_dbeta - L_d50)
		ElseIf param = 2 Then ' Output L_bd50 as in Equation (33)
			diff_loss = L_bfsg + L_d50
		Else  ' Output L_bd as a combination of Equations (32) and (34)
			diff_loss = L_b0p + (L_d50 + F_i * (L_dbeta - L_d50))
		End If
	End If
End Function




Rem Attribute VBA_ModuleType=VBAModule
Option VBASupport 1
Function basic_tx_loss3(p, f, DELTA_N, htg, hrg, theta_path, N0, Gt, Gr, omega, dtm, dlm, dct, dcr, param)
	' This is the main function for calculating the basic transmission loss as per the procedure
	' defined in Recommendation ITU-R P.452-14

	' Inputs:
	' p = % time for which loss is not exceeded (or some Prip' is exceeded)
	' f = frequency (GHz)
	' DELTA_N = refractivity gradient (ref maps in figs 11 & 12 in P.452) (N-units/km)
	' htg = height of TX ant center above ground (m)
	' hrg = height of RX ant center above ground (m)
	' theta_path = path center latitude (deg)
	' N0 = sea-level surface refractivity (N-units) (ref map in fig 13 in P.452)
	' Gt = TX ant gain in direction of interference path (dBi)
	' Gr = RX ant gain in direction of interference path (dBi)
	' omega = Fraction of the path over water (0-1)
	' dtm = longest continuous land (inland + coastal) section of the great-circle path (km)
	' dlm = longest continuous inland section of the great circle path (km)
	' dct = longest continuous land (inland + coastal) section of the great-circle path (km)
	' dcr = Corresponding distance for the second (interfered-with) station (km)

	' Outputs:
	'   Lb = basic tranmission loss (dB)

	' Set some parameters
	pressure = 1013 ' (hPa)
	temp = 15  ' (deg C)
	BETAo = parameter_betao(theta_path, dtm, dlm)

	'          ANALYZE THE PATH PROFILE
	' Section 3.2.1 of Recommendation P.452-14 requires the parameters of Table 3
	' to be derived.
	ae = Path_Profile_Analysis(f, DELTA_N, htg, hrg, 1)
	dtot = Path_Profile_Analysis(f, DELTA_N, htg, hrg, 2)
	dlt = Path_Profile_Analysis(f, DELTA_N, htg, hrg, 14)
	dlr = Path_Profile_Analysis(f, DELTA_N, htg, hrg, 15)
	thetat = Path_Profile_Analysis(f, DELTA_N, htg, hrg, 5)
	thetar = Path_Profile_Analysis(f, DELTA_N, htg, hrg, 6)
	theta = Path_Profile_Analysis(f, DELTA_N, htg, hrg, 7)
	hts = Path_Profile_Analysis(f, DELTA_N, htg, hrg, 3)
	hrs = Path_Profile_Analysis(f, DELTA_N, htg, hrg, 4)
	hte = Path_Profile_Analysis(f, DELTA_N, htg, hrg, 12)
	hre = Path_Profile_Analysis(f, DELTA_N, htg, hrg, 13)
	' Note: db as defined in Table 3 is never used in the Recommendation
	' Note: omega, dct and dcr are all user inputs from the spreadsheet
	PATH_TYPE = Path_Profile_Analysis(f, DELTA_N, htg, hrg, 8)
	hst = Path_Profile_Analysis(f, DELTA_N, htg, hrg, 9)
	hsr = Path_Profile_Analysis(f, DELTA_N, htg, hrg, 10)
	hm = Path_Profile_Analysis(f, DELTA_N, htg, hrg, 11)


	'          PERFORM LINE-OF-SIGHT ATTENUATION CALCULATIONS
	' Section 4.1 of Rec P.452-14 requires the following Line-of-Sight parameters
	' to be calculated
	' Equation (11)
	L_b0p = los_prop(p, f, dlt, dlr, dtot, pressure, temp, BETAo, omega, 1)
	' Equation (12)
	L_b0beta = los_prop(p, f, dlt, dlr, dtot, pressure, temp, BETAo, omega, 2)

	' The result of Equation (8) of Section 4.1 is required as an input to the
	' diffraction calculation in Section 4.2 of Rec P.452-14
	L_bfsg = los_prop(p, f, dlt, dlr, dtot, pressure, temp, BETAo, omega, 3)


	'           PERFORM DIFFACTION CALCULATIONS
	' Section 4.2 of Rec P.452-14 requires the following Diffraction parameters
	' to be calculated
	' Equation (32)
	L_dp = diff_loss(p, f, DELTA_N, hrs, hts, L_bfsg, L_b0p, BETAo, 1)
	' Equation (33)
	L_bd50 = diff_loss(p, f, DELTA_N, hrs, hts, L_bfsg, L_b0p, BETAo, 2)
	' Equation (34)
	L_bd = diff_loss(p, f, DELTA_N, hrs, hts, L_bfsg, L_b0p, BETAo, 3)


	'           PERFORM TROPOSCATTER CALCULATIONS
	' Section 4.3 of Rec P.452-14 requires the following Troposcatter parameter
	' to be calculated
	' Equation (35)
	L_bs = troposcatter_prop(p, f, dtot, pressure, temp, theta, N0, Gt, Gr)


	'           PERFORM DUCTING/LAYER REFLECTION CALCULATIONS
	' Section 4.4 of Rec P.452-14 requires the following Diffraction parameter
	' to be calculated
	' Equation (36)
	L_ba = anomalous_prop(p, f, dtot, pressure, temp, DELTA_N, BETAo, dlm, dlt, dlr, thetat, thetar, hts, hrs, hte, hre, hm, omega, dct, dcr)


	'           COMBINE THE RESULTS FROM ABOVE INTO THE OVERAL PREDICTION
	' Perform the calculations as given in Section 4.6 of Rec P.452-14
	basic_tx_loss3 = overall_predict(p, theta, dtot, BETAo, omega, L_b0p, L_b0beta, L_dp, L_bd50, L_bd, L_bs, L_ba)

End Function





Rem Attribute VBA_ModuleType=VBAModule
Option VBASupport 1
Function inv_cum_norm(p)

	' Function to approximate inverse cumulative normal distribution function
	' This function is valid for 0.000001 <= p <= 0.5 and is in error by a
	' maximum of 0.00054. If p<0.000001, p is set to 0.000001.

	' Reference: ITU-R P.452-14 Appendix 3 (Propagation by Diffraction)

	' Inputs:
	' p = the input probability (a fraction)

	' Output:
	' The value of the Gaussian normal rv "X" which satisfys Prob(X<=Y)=p

	If (p < 0.000001) Then
		p = 0.000001
	End If

	C0 = 2.515516698  ' Equation (158c)
	C1 = 0.802853  ' Equation (158d)
	C2 = 0.010328  ' Equation (158e)
	D1 = 1.432788  ' Equation (158f)
	D2 = 0.189269  ' Equation (158g)
	D3 = 0.001308  ' Equation (158h)

	N_inv_cum_norm = ((C2 * Tsub(p) + C1) * Tsub(p)) + C0
	D_inv_cum_norm = ((D3 * Tsub(p) + D2) * Tsub(p) + D1) * Tsub(p) + 1
	chi = N_inv_cum_norm / D_inv_cum_norm  '  Equation (158b)
	inv_cum_norm = chi - Tsub(p)  ' Equation (158)
End Function

Function Tsub(x)   ' subfunction
	Tsub = Sqr(-2 * Log(x))  'Equation (158a)
End Function





Rem Attribute VBA_ModuleType=VBAModule
Option VBASupport 1
Function Jnu(nu)

	' Function to compute the knife-edge diffraction loss J(nu) in section
	' 4.2 of P.452-14

	' Inputs:
	' nu = the input parameter

	' Outputs:
	' The value of J(nu)

	If (nu < -0.78) Then

		Jnu = 0
	Else
		' Equation (13)
		Jnu = 6.9 + 20 * Log(Sqr((nu - 0.1) ^ 2 + 1) + nu - 0.1) / Log(10)
	End If

End Function
Function num(x)
	' converts dBs to numeric (non-dB)

	num = 10 ^ (x / 10)

End Function
Function dB(x)
	' converts numbers to dBs
	' (Note: In Visual BASIC, Log is a natural logrithm.  There is no intrinsic base 10 log.)
	dB = 10 * Log(x) / Log(10)
End Function






Rem Attribute VBA_ModuleType=VBAModule
Option VBASupport 1
Function AZ_EL_RANGE(H, thetas, phis, thetae, phie, param)
	Application.Volatile

	' This function computes the azimuth,elevation, and range of a satellite
	' at altitude H and SSP (thetas,phis) as seen from an Earth site at (thetae,phie)
	' H is in km
	' Theta is Latitude (degrees N)
	' Phi is Longitude (degrees E)

	Re = 6378.153           'Earth radius (km)
	deg = 4 * Atn(1) / 180 'deg-to-rad conversion factor

	' We define a geocentric LAT/LON coordinate system whose origin is at the Earth's center;
	' +x-direction (I unit vector) towards 0°E (prime meridian); +y-direction (J unit vector)
	' towards 90°E; and +z-direction (K unit vector) towards the North pole (90°N) to form
	' a right-handed Cartesian system. In this IJK system, the {xyz} coordinates of the
	' satellite and earth station are:

	R = Re + H

	xs = R * Cos(thetas * deg) * Cos(phis * deg)
	ys = R * Cos(thetas * deg) * Sin(phis * deg)
	zs = R * Sin(thetas * deg)

	xe = Re * Cos(thetae * deg) * Cos(phie * deg)
	ye = Re * Cos(thetae * deg) * Sin(phie * deg)
	ze = Re * Sin(thetae * deg)

	' Now define a topocentric coordinate system on the Earth's surface whose origin is
	' at the E/S; +x-direction pointed due South (S); +y-direction pointed due East (E);
	' and +z-direction pointed towards the zenith (Z) to form a right-handed system.
	' Suppose we know the {IJK} components of some vector V in the lat/lon system and wish
	' to find its {SEZ} components in the topocentric system. We can do this by first
	' rotating the IJK frame through a positive angle phie about the lat/lon z-axis (K)
	' and then rotating it through a positive angle (90-thetae) about the lat/lon y-axis
	' (J). This will bring it into angular alignment with the SEZ frame. This transformation
	' is represented by the 3 x 3 matrix


	'M = [sin(thetae*deg)*cos(phie*deg)  sin(thetae*deg)*sin(phie*deg)  -cos(thetae*deg);
	'        -sin(phie*deg)                     cos(phie*deg)                 0         ;
	'     cos(thetae*deg)*cos(phie*deg)  cos(thetae*deg)*sin(phie*deg)    sin(thetae*deg)];
	
	Dim M0 As Variant
	Dim M1 As Variant
	Dim M2 As Variant

	M0 = Array(Sin(thetae * deg) * Cos(phie * deg), -Sin(phie * deg), Cos(thetae * deg) * Cos(phie * deg))
	M1 = Array(Sin(thetae * deg) * Sin(phie * deg), Cos(phie * deg), Cos(thetae * deg) * Sin(phie * deg))
	M2 = Array(-Cos(thetae * deg), 0, Sin(thetae * deg))
	
	' If we let Vs = [xs ys zs] be the vector from Earth center to satellite;
	' Ve = [xe ye ze] be the vector from Earth center to Earth station; and
	' rho be the vector from E/S to satellite; then Vs=Ve+rho and rho=Vs-Ve
	' In terms of IJK components, rho is therefore given by

	rho_I = xs - xe
	Rho_J = ys - ye
	Rho_K = zs - ze

	' Transforming to SEZ components using matrix M above we have
	Rho_S = M0(0) * rho_I + M1(0) * Rho_J + M2(0) * Rho_K
	Rho_E = M0(1) * rho_I + M1(1) * Rho_J + M2(1) * Rho_K
	rho_Z = M0(2) * rho_I + M1(2) * Rho_J + M2(2) * Rho_K


	' From this vector and the geometry of the SEZ system, we have
	Rnge = Sqr(Rho_S ^ 2 + Rho_E ^ 2 + rho_Z ^ 2)
	EL = Asin(rho_Z / Rnge) / deg

	If Rho_S = 0 Then
		AZ = 0
	Else
		AZ = Atan2(Rho_E, -Rho_S) / deg
	End If

	If AZ < 0 Then
		AZ = AZ + 360
	End If

	If param = 1 Then
		AZ_EL_RANGE = AZ
	ElseIf param = 2 Then
		AZ_EL_RANGE = EL
	Else
		AZ_EL_RANGE = Rnge
	End If

End Function
Function Asin(x)
	' This function calculates the inverse sine function as it is not intrinsic to Visual Basic
	If x = 1 Then
		Asin = 2 * Atn(1)
	Else
		Asin = Atn(x / Sqr(-x * x + 1))
	End If
End Function
Function Acos(x)
	' This function calculates the inverse cosine function as it is not intrinsic to Visual Basic
	If x = 1 Then
		Acos = 0
	Else
		Acos = Atn((-1 * x) / Sqr(-x * x + 1)) + 2 * Atn(1)
	End If
End Function
Function Atan2(x, Y)
	' This function calculates the inverse tangent function but also corrects
	' for the proper quadrant as the Atn intrinsic to Visual Basic does not
	' take it into account
	If Y >= 0 Then
		Atan2 = Atn(x / Y)
	ElseIf x < 0 Then
		Atan2 = Atn(x / Y) - 4 * Atn(1)
	Else
		Atan2 = Atn(x / Y) + 4 * Atn(1)
	End If
End Function







Rem Attribute VBA_ModuleType=VBAModule
Option VBASupport 1
Option Base 1
Function overall_predict(p, theta, dtot, BETAo, omega, L_b0p, L_b0beta, L_dp, L_bd50, L_bd, L_bs, L_ba)
	' Implements section 4.6 of Rec. P.452-14.  Written by Glenn Dec 07, updated Dec 09 and June 10

	'Inputs:
	'   theta:path angular distance (mrad)
	'   dtot: the total great circle path distance between the terminals (km)
	'   BETAo: The time percentage for which refractive index lapse-rates exceeding
	'       100 N-units/km can be expected in the first 100 m of lower atmosphere.
	'   omega: fraction of the path over water (0-1)
	'   L_b0p: Basic transmission loss not exceeded for p% due to
	'       line-of-sight propagation (dB)
	'   L_b0beta: Basic transmission loss not exceeded for beta% due to
	'       line-of-sight propagation (dB)
	'   L_dp: The diffraction loss not exceeded for p% time (dB)
	'   L_bd50: The median basic transmission loss associated with diffraction (dB)
	'   L_bd: The basic transmission loss associated with diffraction not exceeded p% (dB)
	'   L_bs: The basic transmission loss due to troposcatter not exceeded  p% (dB)
	'   L_ba: The basic transmission loss due to periods of anomalous propagation (dB)

	' Output:
	'   L_b: Basic transmission loss not exceeded p% of time

	' Calculate interpolation factor, F_j, as per Equation (48)
	BIG_THETA = 0.3
	xi = 0.8
	F_j = 1# - 0.5 * (1# + Tanh(3# * xi * (theta - BIG_THETA) / BIG_THETA))

	' Calculate interpolation factor, F_k, as per Equation (49)
	d_sw = 20  ' fixed parameter determing the distance range of blending
	kappa = 0.5  ' fixed parameter determing the blending slope
	F_k = 1# - 0.5 * (1# + Tanh(3# * kappa * (dtot - d_sw) / d_sw))

	' Calculate a notional minimum basic transmission loss, L_minb0p, associated with
	' line-of-sight propagation and over-sea sub-path diffraction as per Equation (50) (dB)
	If p < BETAo Then
		L_minb0p = L_b0p + (1 - omega) * L_dp
	Else
		' Calculation of F_i added 2 June 2010
		If p = 50 Then
			F_i = 0
		ElseIf 50 > p And p > BETAo Then
			F_i = inv_cum_norm(p / 100) / inv_cum_norm(BETAo / 100)
		Else
			F_i = 1
		End If
		L_minb0p = L_bd50 + (L_b0beta + (1 - omega) * L_dp - L_bd50) * F_i
	End If

	' Calculate a notional minimum basic transmission loss, L_minbap, associated with
	' line-of-sight and transhorizon signal enhancements as per Equation (51) (dB)
	eta = 2.5
	L_minbap = eta * Log(Exp(L_ba / eta) + Exp(L_b0p / eta))

	' Calculate a notional basic transmission loss, L_bda, associated with diffraction
	' and line-of-sight or ducting/layer-reflection enhancements as per Equation (52) (dB)
	If L_minbap > L_bd Then
		L_bda = L_bd
	Else
		L_bda = L_minbap + (L_bd - L_minbap) * F_k
	End If

	' Calculate a modified basic transmission loss, L_bam, which takes diffraction and
	' line-of-sight or ducting/layer-reflection enhancements into account as per Equation (53) (dB)
	L_bam = L_bda + (L_minb0p - L_bda) * F_j

	' Calculate the final basic transmission loss, L_b, not exceed for p% time
	' as per Equation (54) (dB)
	' Note: In this spreadsheet implementation, clutter losses A_ht and A_hr are ignored
	A_ht = 0
	A_hr = 0
	L_b = -5 * Log(10 ^ (-0.2 * L_bs) + 10 ^ (-0.2 * L_bam)) / Log(10) + A_ht + A_hr

	overall_predict = L_b
End Function







Rem Attribute VBA_ModuleType=VBAModule
Option VBASupport 1
Function correct_smooth_earth(dtot, hst, hgt, hsr, hgr, m, param)

	' Function used by path profile analysis routine

	' Reference: ITU-R P.452-14 Appendix 2 to Annex 1, Section 5.1.6.3

	' Inputs:
	' dtot = the total great circle path distance between the two terminals(km)
	' hst = (uncorrected) height of the smooth earth surface (amsl) at TX (m)
	' hgt = ground height (amsl) at TX station (m)
	' hsr = (uncorrected) height of smooth earth surface at RX station (m)
	' hgr = ground height (amsl) at RX station (m)
	' m = slope of the least-squares surface relative to sea level (m/km)

	' Outputs:
	' If param=1 then hst = corrected height of smooth earth surface at TX (m)
	' If param=2 then hsr = corrected height of smooth earth surface at RX (m)
	' If param=3 then m = corrected slope (m/km)

	x = 0
	' Equation (155a) sort of
	If (hst > hgt) Then
		x = 1
		hst = hgt
	End If
	' Equation (155b) sort of
	If (hsr > hgr) Then
		x = 1
		hsr = hgr
	End If
	If (x = 1) Then
		' Equation (156)
		m = (hsr - hst) / dtot
	End If

	If param = 1 Then
		correct_smooth_earth = hst
	ElseIf param = 2 Then
		correct_smooth_earth = hsr
	Else
		correct_smooth_earth = m
	End If
End Function






Rem Attribute VBA_ModuleType=VBAModule
Option VBASupport 1
Function troposcatter_prop(p, f, dtot, pressure, temp, theta, N0, Gt, Gr)

	' Function to compute basic transmission loss due to troposcatter effects
	' Loss is not exceeded for p% of the time (p<=50%). This loss needs to be calculated
	' for transhorizon paths

	' Reference: ITU-R P.452-14 Section 4.3

	' Inputs:
	' p = the % time for which the loss is NOT exceeded (note: this model is
	'     only valid for p <= 50%. It will return a complex number if p>50%.)
	' f = frequency (GHz)
	' dtot = great circle path distance between the two stations (km)
	' pressure = atmospheric pressure (hPa) (e.g. typical value is 1013 hPa)
	' temp = mean air temp (deg Centigrade)
	' THETA = the trans-horizon path angular distance (mrad)
	' N0 = the path center sea-level surface refractivity (N-units)(see Fig 13 in P452)
	' Gt = TX ant gain in direction of its horizon vector along int path (dBi)
	' Gr = RX ant gain in direction of its horizon vector along int path (dBi)

	' Outputs:
	' troposcatter_prop = basic TX loss NOT exceeded for p% of time (p<=50%) due to troposcatter (dB)

	'water vapor density (g/m^3) - See condition under Equation (35b)
	rho = 3

	' specific att for dry air (dB/km) - Calculated from Rec P.676-6 Annex 2
	gammao = specific_atten_dryair(f, pressure, temp)

	' specific att for water vapor (dB/km) - Calculated from Rec P.676-6 Annex 2
	gammaw = specific_atten_water(f, pressure, temp, rho)

	' total gaseous atten (dB) - Equation (8)
	Ag = (gammao + gammaw) * dtot

	' frequency dependent loss (dB) - Equation (35a)
	Lf = 25 * Log(f) / Log(10) - 2.5 * (Log(f / 2) / Log(10)) ^ 2

	' aperture-to-medium coupling loss (dB) - Equation (35b)
	Lc = 0.051 * Exp(0.055 * (Gt + Gr))

	' Equation (35)
	troposcatter_prop = 190 + Lf + 20 * Log(dtot) / Log(10) + 0.573 * theta - 0.15 * N0 + Lc + Ag - 10.1 * (-1 * Log(p / 50) / Log(10)) ^ 0.7
End Function

Function los_prop(p, f, dlt, dlr, dtot, pressure, temp, BETAo, omega, param)

	' Function to compute LOS propagation basic transmission loss NOT exceeded
	' for p% of the time

	' Reference: ITU-R P.452-14 Section 4.1

	' Inputs:
	' p = the % time for which the basic TX loss is NOT exceeded
	' f = frequency (GHz) note: this routine applies only for f<=54 GHz
	'                           for f>54 GHz, see Annex 2 of P.676-6
	' dlt = For a transhorizon path, distance from the transmit antenna to the horizon (km).
	'       For a line-of-sight path, the distance from the terminal to the profile point
	'       identified as the principal edge in the diffraction method for 50% time.
	' dlr = For a transhorizon path, distance from the receive antenna to the horizon (km).
	'       For a line-of-sight path, the distance from the terminal to the profile point
	'       identified as the principal edge in the diffraction method for 50% time.
	' dtot = total path length (km)
	' pressure = atmospheric pressure (hPa) (e.g. typical value is 1013 hPa)
	' temp = mean air temp (deg Centigrade) (can be obtained from maps in Rec
	'        P.1510 if necessary. Typical value is 15 deg C)
	' BETAo = the time percentage for which refractive index lapse-rates exceeding 100 N units/km
	'       can be expected in the first 100 m of the lower atmosphere
	' omega = the fraction of the total interference path over water
	'         (i.e. omega=0 for totally overland path and omega=1 for totally
	'         over water path).  This parameter is only required for worst-month
	'         analysis.  (See cell F13 on Step_2.)
	' param = Output desired where
	'       1 = L_b0p (dB) the basic TX loss NOT exceeded for p% of time due to LOS prop in Equation (11)
	'       2 = L_b0beta (dB) the basic TX loss not exceeded for beta% of time due to LOS prop in Equation (12)
	'       3 = L_bfsg (dB) the basic TX loss due to free-space prop and atten by atmos gases in Equation (8)

	'rho: water vapor density (g/m^3) - Equation (9a)
	rho = 7.5 + 2.5 * omega

	'gammao: specific att for dry air (dB/km) - Calculated from Rec P.676-6 Annex 2
	gammao = specific_atten_dryair(f, pressure, temp)

	' gammaw: specific att for water vapor (dB/km) - Calculated from Rec P.676-6 Annex 2
	gammaw = specific_atten_water(f, pressure, temp, rho)

	' Ag: total gaseous atten (dB) - Equation (9)
	Ag = (gammao + gammaw) * dtot

	' Basic TX prop loss (dB) due to free space and gaseous absorption - Equation (8)
	L_bfsg = 92.5 + 20 * Log(f) / Log(10) + 20 * Log(dtot) / Log(10) + Ag

	If param = 1 Then
		' E_sp: correction for multipath and focussing effects at p% time (dB) - Equation (10a)
		E_sp = 2.6 * (1 - Exp(-0.1 * (dlt + dlr))) * Log(p / 50) / Log(10)
		los_prop = L_bfsg + E_sp  ' Equation (11)
	ElseIf param = 2 Then
		' E_sbeta: correction for multipath and focussing effects at beta% time (dB) - Equation (10b)
		E_sbeta = 2.6 * (1 - Exp(-0.1 * (dlt + dlr))) * Log(BETAo / 50) / Log(10)
		los_prop = L_bfsg + E_sbeta  'Equation (12)
	Else
		los_prop = L_bfsg
	End If
End Function
Function specific_atten_dryair(f, pressure, temp)
	Application.Volatile

	' Function to compute specific attenuation (dB/km) due to dry air

	' Reference: ITU-R P.676-6 Annex 2

	' Inputs:
	' f = frequency (GHz) note: this routine applies only for f<=54 GHz
	'                           for f>54 GHz, see Annex 2 of P.676-6
	' pressure = atmospheric pressure (hPa) (e.g. typical value is 1013 hPa)
	' temp = mean air temp (deg Centigrade) (can be obtained from maps in Rec
	'        P.1510 if necessary. Typical value is 15 deg C)

	' Outputs:
	' specific_atten_dryair = specific attenuation due to dry air (dB/km)

	T = temp
	p = pressure
	rp = p / 1013
	rt = 288 / (273 + T)

	z1 = psi_specific_atten(rp, rt, 0.0717, -1.8132, 0.0156, -1.6515)

	z2 = psi_specific_atten(rp, rt, 0.5146, -4.6368, -0.1921, -5.7416)

	z3 = psi_specific_atten(rp, rt, 0.3414, -6.5851, 0.213, -8.5854)

	' Equation (22a)
	specific_atten_dryair = (7.2 * rt ^ 2.8 / (f ^ 2 + 0.34 * rp ^ 2 * rt ^ 1.6) + 0.62 * z3 / ((54 - f) ^ (1.16 * z1) + 0.83 * z2)) * f ^ 2 * rp ^ 2 * 0.001

End Function
Function specific_atten_water(f, pressure, temp, rho)
	Application.Volatile

	' Function to compute specific attenuation (dB/km) due to water vapor

	' Reference: ITU-R P.676-6 Annex 2 Eqn's (23a) through (23f)

	' Inputs:
	' f = frequency (GHz) note: this routine applies only for f<=54 GHz
	'                           for f>54 GHz, see Annex 2 of P.676-6
	' pressure = atmospheric pressure (hPa) (e.g. typical value is 1013 hPa)
	' temp = mean air temp (deg Centigrade) (can be obtained from maps in Rec
	'        P.1510 if necessary. Typical value is 15 deg C)
	' rho = the water vapor density (g/m^3) (typical value is 7.5 g/m^3)

	' Outputs:
	' specific_atten_water = specific attenuation due to water vapor (dB/km)

	T = temp
	p = pressure
	rp = p / 1013
	rt = 288 / (273 + T)

	' Equation (23b)
	eta1 = 0.955 * rp * rt ^ 0.68 + 0.006 * rho
	' Equation (23c)
	eta2 = 0.735 * rp * rt ^ 0.5 + 0.0353 * rt ^ 4 * rho

	' This is collectively Equation (23a)
	T1 = (3.98 * eta1 * Exp(2.23 * (1 - rt))) / ((f - 22.235) ^ 2 + 9.42 * eta1 ^ 2) * g_specific_atten(f, 22)
	T2 = (11.96 * eta1 * Exp(0.7 * (1 - rt))) / ((f - 183.31) ^ 2 + 11.14 * eta1 ^ 2)
	T3 = (0.081 * eta1 * Exp(6.44 * (1 - rt))) / ((f - 321.226) ^ 2 + 6.29 * eta1 ^ 2)
	T4 = (3.66 * eta1 * Exp(1.6 * (1 - rt))) / ((f - 325.153) ^ 2 + 9.22 * eta1 ^ 2)
	T5 = (25.37 * eta1 * Exp(1.09 * (1 - rt))) / ((f - 380) ^ 2)
	T6 = (17.4 * eta1 * Exp(1.46 * (1 - rt))) / ((f - 448) ^ 2)
	T7 = (844.6 * eta1 * Exp(0.17 * (1 - rt))) / ((f - 557) ^ 2) * g_specific_atten(f, 557)
	T8 = (290 * eta1 * Exp(0.41 * (1 - rt))) / ((f - 752) ^ 2) * g_specific_atten(f, 752)
	T9 = (83328# * eta2 * Exp(0.99 * (1 - rt))) / ((f - 1780) ^ 2) * g_specific_atten(f, 1780)

	specific_atten_water = (T1 + T2 + T3 + T4 + T5 + T6 + T7 + T8 + T9) * f ^ 2 * rt ^ 2.5 * rho * 0.0001

End Function
Function g_specific_atten(f, fi)
	Application.Volatile

	' This is a small supporting function for the "specific_atten_water" routine
	' Reference: ITU-R P.676-6 Annex 2 Equations (23d)

	g_specific_atten = 1 + ((f - fi) / (f + fi)) ^ 2

End Function
Function psi_specific_atten(rp, rt, a, b, c, D)

	' This is a small supporting function for the "specific_atten_dryair" routine
	' Reference: ITU-R P.676-6 Annex 2 Equation (22u)

	psi_specific_atten = rp ^ a * rt ^ b * Exp((c * (1 - rp) + D * (1 - rt)))
End Function
Function equiv_annual_percent(pw, theta, omega)

	' Function to compute equivalent annual time %p of interference exceedance (or % time that basic TX loss
	' is NOT exceeded) for a given worst month time percentage (pw%)

	' Reference: ITU-R P.452-14 Section 3.2.1

	' Inputs:
	' pw = the worst month time % (%)
	' theta = the interference path center latitude (deg)
	' omega = the fraction of the interference path over water (fraction) (See cell F13 on Step_2.)

	' Outputs:
	' p = the equivalent annual time percentage corresponding to the worst
	' month time percentage (%)

	deg = 4 * Atn(1) / 180#

	' Equation (1a)
	If (Abs(theta) <= 45) Then
		GL = Sqr(1.1 + (Abs(Cos(2 * theta * deg))) ^ 0.7)
	Else
		GL = Sqr(1.1 - (Abs(Cos(2 * theta * deg))) ^ 0.7)
	End If

	' Equation (1)
	p = 10 ^ (((Log(pw) / Log(10)) + (Log(GL) / Log(10)) - 0.186 * omega - 0.444) / (0.816 + 0.078 * omega))
	' See condition under Equation (1a)
	If (12 * p < pw) Then
		p = pw / 12
	End If
	equiv_annual_percent = p

End Function
Function anomalous_prop(p, f, dtot, pressure, temp, DELTA_N, BETAo, dlm, dlt, dlr, theta_t, theta_r, hts, hrs, hte, hre, hm, omega, dct, dcr)

	' Function to compute basic transmission loss due to ducting and
	' atmospheric layer reflection/refraction losses.
	' Loss is not exceeded for p% of the time. This loss needs to be calculated
	' for transhorizon paths

	' Reference: ITU-R P.452-14 Section 4.4

	' Inputs:
	' p = the % time for which the basic transmission loss is NOT exceeded
	' f = frequency (GHz)
	' dtot = great circle path distance between the two stations (km)
	' pressure = atmospheric pressure (hPa) (e.g. typical value is 1013 hPa)
	' temp = mean air temp (deg Centigrade)
	' DELTA_N = the mean refractivity gradient through the lowest 1km of the
	'           atmosphere (N-units/km). See maps in Figs 11 and 12 of P452)
	' BETAo = the % time for which DELTA_N is expected to exceed 100 N-units/km
	'         in the lowest 100 m of the atmosphere
	' dlm = longest continuous inland section of the great circle path (km)
	' dlt = great circle distance from TX station to its horizon point (km)
	' dlr = great circle distance from RX station to its horizon point (km)
	' theta_t = TX station horizon elevation angle (mrad)
	' theta_r = RX station horizon elevation angle (mrad)
	' hts = TX ant center height amsl (m)
	' hrs = RX ant center height amsl (m)
	' hte = effective height of TX ant above the local terrain (m)
	' hre = effective height of RX ant above the local terrain (m)
	' hm = terrain roughness parameter (m)
	' omega = fraction of total interference path over water
	' dct = for a path which has one or more sections over water, the distance
	'       OVER LAND from the TX station to the coast along the path (km). For
	'       a terminal onboard a ship or sea platform dct = 0. The exact value
	'       of dct is only important if dct <= 5 km.
	' dcr = for a path which has one or more sections over water, the distance
	'       OVER LAND from the RX station to the coast along the path (km). For
	'       a terminal onboard a ship or sea platform dcr = 0. The exact value
	'       of dcr is only important if dcr <= 5 km.

	' Outputs:
	' Basic TX loss NOT exceeded for p% of time due to anomalous prop effects (dB)

	' water vapor density (g/m^3) - Equation (9a)
	rho = 7.5 + 2.5 * omega

	' specific att for dry air (dB/km) - Calculated using Rec P.676-6 Annex 2
	gammao = specific_atten_dryair(f, pressure, temp)
	' specific att for water vapor (dB/km)  - Calculated using Rec P.676-6 Annex 2
	gammaw = specific_atten_water(f, pressure, temp, rho)
	' total gaseous atten (dB) - Equation (18)
	Ag = (gammao + gammaw) * dtot

	' Equations (39) and (39a)
	Act = 0
	If ((omega >= 0.75) And (dct <= dlt) And (dct <= 5)) Then
		Act = -3 * Exp(-0.25 * dct ^ 2) * (1 + Tanh(0.07 * (50 - hts)))
	End If

	Acr = 0
	If ((omega >= 0.75) And (dcr <= dlr) And (dcr <= 5)) Then
		Acr = -3 * Exp(-0.25 * dcr ^ 2) * (1 + Tanh(0.07 * (50 - hrs)))
	End If

	' Equation (38a)
	theta_t2 = theta_t - 0.1 * dlt
	theta_r2 = theta_r - 0.1 * dlr

	' Equation (38)
	If (theta_t2 > 0) Then
		Ast = 20 * Log(1 + 0.361 * theta_t2 * Sqr(f * dlt)) / Log(10) + 0.264 * theta_t2 * f ^ (1 / 3)
	Else
		Ast = 0
	End If

	If (theta_r2 > 0) Then
		Asr = 20 * Log(1 + 0.361 * theta_r2 * Sqr(f * dlr)) / Log(10) + 0.264 * theta_r2 * f ^ (1 / 3)
	Else
		Asr = 0
	End If

	' Equation (37a) - Note this was one primary addition between Rec P.452-13 and P.452-14
	If (f < 0.5) Then
		Alf = 45.375 - 137 * f + 92.5 * f ^ 2
	Else
		Alf = 0
	End If

	' Equation (37)
	Af = 102.45 + 20 * Log(f) / Log(10) + 20 * Log(dlt + dlr) / Log(10) + Alf + Ast + Asr + Act + Acr

	' Equation (46a)
	If dtot - dlt - dlr < 40 Then
		Di = dtot - dlt - dlr
	Else
		Di = 40
	End If
	' Equation (46)
	If (hm <= 10) Then
		mu3 = 1
	Else
		mu3 = Exp(-0.000046 * (hm - 10) * (43 + 6 * Di))
	End If

	' Equations (3a) and (45a)
	tau = 1 - Exp(-0.000412 * dlm ^ 2.41)
	eps = 3.5
	alpha = -0.6 - eps * 0.000000001 * dtot ^ 3.1 * tau
	If (alpha < -3.4) Then
		alpha = -3.4
	End If

	k50 = median_eff_Re(DELTA_N, 1)
	ae = median_eff_Re(DELTA_N, 2)

	'Equation (45)
	mu2 = (500 / ae * dtot ^ 2 / (Sqr(hte) + Sqr(hre)) ^ 2) ^ alpha
	If (mu2 > 1) Then
		mu2 = 1
	End If

	' Equation (44)
	beta = BETAo * mu2 * mu3
	' Equation (43a)
	GAMMA = 1.076 / (2.0058 - Log(beta) / Log(10)) ^ 1.012 * Exp(-(9.51 - 4.8 * Log(beta) / Log(10) + 0.198 * (Log(beta) / Log(10)) ^ 2) * 0.000001 * dtot ^ 1.13)
	' Equation (43)
	Ap = -12 + (1.2 + 0.0037 * dtot) * Log(p / beta) / Log(10) + 12 * (p / beta) ^ GAMMA

	' Equation (42a)
	If (theta_t <= 0.1 * dlt) Then
		theta_t1 = theta_t
	Else
		theta_t1 = 0.1 * dlt
	End If

	If (theta_r <= 0.1 * dlr) Then
		theta_r1 = theta_r
	Else
		theta_r1 = 0.1 * dlr
	End If

	' Equation (42)
	theta_1 = 1000 * dtot / ae + theta_t1 + theta_r1
	' Equation (41)
	gammad = 0.00005 * ae * f ^ (1 / 3)
	' Equation (40)
	Ad = gammad * theta_1 + Ap

	' Equation (36)
	anomalous_prop = Af + Ad + Ag

End Function

Function Tanh(x)
	' This is a trig identity used because Visual Basic does not have
	' an intrinsic hyperbolic tangent function.
	' The IF/THEN code is to avoid exp(X) getting too large and crashing
	' Note: tanh(10)=0.999999995878

	If x > 10 Then
		Tanh = 1
	ElseIf x < -10 Then
		Tanh = -1
	Else
		Tanh = (Exp(x) - Exp(-1 * x)) / (Exp(x) + Exp(-1 * x))
	End If
End Function
