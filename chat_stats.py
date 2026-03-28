import re
import os
import sys
from datetime import datetime, timedelta
from collections import Counter

try:
    from openpyxl import Workbook
    from openpyxl.styles import Font, Alignment, Border, Side, PatternFill
except ImportError:
    print("openpyxl이 설치되어 있지 않습니다. 설치합니다...")
    os.system(f"{sys.executable} -m pip install openpyxl")
    from openpyxl import Workbook
    from openpyxl.styles import Font, Alignment, Border, Side, PatternFill


def parse_chat_log(file_path, days=30):
    """카카오톡 채팅 로그를 파싱하여 사용자별 메시지 수를 집계합니다."""

    with open(file_path, "r", encoding="utf-8") as f:
        content = f.read()

    today = datetime.now().replace(hour=0, minute=0, second=0, microsecond=0)
    start_date = today - timedelta(days=days)

    # 날짜 구분선 패턴: --------------- 2026년 3월 24일 화요일 ---------------
    date_pattern = re.compile(
        r"-{2,}\s+(\d{4})년\s+(\d{1,2})월\s+(\d{1,2})일\s+\S+요일\s+-{2,}"
    )

    # 메시지 패턴: [닉네임/게임닉/숫자/문자] [오전/오후 시:분] 메시지
    # 또는 [닉네임] [오전/오후 시:분] 메시지
    msg_pattern = re.compile(
        r"^\[([^\]]+)\]\s+\[(?:오전|오후)\s+\d{1,2}:\d{2}\]\s+"
    )

    current_date = None
    user_counter = Counter()
    # 사용자 식별자(게임닉) -> 실명 매핑
    user_info = {}

    for line in content.split("\n"):
        line = line.strip()
        if not line:
            continue

        # 날짜 구분선 체크
        date_match = date_pattern.search(line)
        if date_match:
            year = int(date_match.group(1))
            month = int(date_match.group(2))
            day = int(date_match.group(3))
            current_date = datetime(year, month, day)
            continue

        # 날짜가 범위 밖이면 스킵
        if current_date is None:
            continue
        if current_date < start_date or current_date > today:
            continue

        # 메시지 매칭
        msg_match = msg_pattern.match(line)
        if msg_match:
            sender_raw = msg_match.group(1)

            # [오픈채팅봇] 같은 봇 메시지 제외
            if sender_raw in ("오픈채팅봇",):
                continue

            # sender_raw 예시: "김정은/00년생 혐오증/97/P"
            # 또는 "물리적데미지감소/정재진/89/E"
            parts = sender_raw.split("/")
            if len(parts) >= 2:
                # 실명과 게임닉 추출
                real_name = parts[0].strip()
                game_nick = parts[1].strip()
                display = f"{real_name}/{game_nick}"
            else:
                display = sender_raw.strip()

            user_counter[display] += 1

    return user_counter


def create_excel(user_counter, output_path):
    """사용자별 메시지 통계를 엑셀 파일로 저장합니다."""

    wb = Workbook()
    ws = wb.active
    ws.title = "채팅 통계"

    # 스타일 정의
    header_font = Font(bold=True, size=12, color="FFFFFF")
    header_fill = PatternFill(start_color="4472C4", end_color="4472C4", fill_type="solid")
    header_align = Alignment(horizontal="center", vertical="center")
    thin_border = Border(
        left=Side(style="thin"),
        right=Side(style="thin"),
        top=Side(style="thin"),
        bottom=Side(style="thin"),
    )

    # 제목 행
    today = datetime.now()
    start = today - timedelta(days=30)
    ws.merge_cells("A1:C1")
    title_cell = ws["A1"]
    title_cell.value = f"채팅 통계 ({start.strftime('%Y-%m-%d')} ~ {today.strftime('%Y-%m-%d')})"
    title_cell.font = Font(bold=True, size=14)
    title_cell.alignment = Alignment(horizontal="center")

    # 헤더
    headers = ["순위", "사용자 (실명/게임닉)", "메시지 수"]
    for col, header in enumerate(headers, 1):
        cell = ws.cell(row=3, column=col, value=header)
        cell.font = header_font
        cell.fill = header_fill
        cell.alignment = header_align
        cell.border = thin_border

    # 데이터 (메시지 수 내림차순)
    sorted_users = user_counter.most_common()
    for rank, (user, count) in enumerate(sorted_users, 1):
        row = rank + 3
        ws.cell(row=row, column=1, value=rank).border = thin_border
        ws.cell(row=row, column=1).alignment = Alignment(horizontal="center")
        ws.cell(row=row, column=2, value=user).border = thin_border
        ws.cell(row=row, column=3, value=count).border = thin_border
        ws.cell(row=row, column=3).alignment = Alignment(horizontal="center")

    # 합계 행
    total_row = len(sorted_users) + 4
    ws.cell(row=total_row, column=1, value="").border = thin_border
    total_label = ws.cell(row=total_row, column=2, value="합계")
    total_label.font = Font(bold=True)
    total_label.border = thin_border
    total_val = ws.cell(row=total_row, column=3, value=sum(user_counter.values()))
    total_val.font = Font(bold=True)
    total_val.alignment = Alignment(horizontal="center")
    total_val.border = thin_border

    # 열 너비 조정
    ws.column_dimensions["A"].width = 8
    ws.column_dimensions["B"].width = 35
    ws.column_dimensions["C"].width = 12

    wb.save(output_path)
    print(f"엑셀 파일 저장 완료: {output_path}")


def main():
    if len(sys.argv) < 2:
        print("사용법: python chat_stats.py <카카오톡_채팅로그.txt> [출력파일.xlsx]")
        print("예시: python chat_stats.py chat.txt 채팅통계.xlsx")
        sys.exit(1)

    input_file = sys.argv[1]
    if not os.path.exists(input_file):
        print(f"파일을 찾을 수 없습니다: {input_file}")
        sys.exit(1)

    output_file = sys.argv[2] if len(sys.argv) >= 3 else "채팅통계.xlsx"

    print(f"채팅 로그 분석 중: {input_file}")
    user_counter = parse_chat_log(input_file, days=30)

    if not user_counter:
        print("최근 30일 내 메시지가 없습니다.")
        sys.exit(0)

    print(f"총 {len(user_counter)}명의 사용자, {sum(user_counter.values())}개의 메시지 발견")
    create_excel(user_counter, output_file)


if __name__ == "__main__":
    main()
