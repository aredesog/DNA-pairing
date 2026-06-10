from pathlib import Path
import re
import subprocess


BASE_DIR = Path(__file__).resolve().parent
INSTANCES_DIR = BASE_DIR.parent / "tp_02" / "instancias"
OUTPUT_CSV = BASE_DIR / "resultados.csv"


def find_executable() -> Path:
    candidatos = [BASE_DIR / "base.exe", BASE_DIR / "base"]
    for candidato in candidatos:
        if candidato.exists():
            return candidato
    raise FileNotFoundError("Nao encontrei o executavel 'base' ou 'base.exe' em src/")


def extract_instance_key(filename: str) -> tuple[int, int]:
    match = re.match(r"^(\d+)-(\d+)$", filename)
    if not match:
        return (0, 0)
    return (int(match.group(1)), int(match.group(2)))


def parse_output(output: str) -> dict[str, str | None]:
    regex_mem_dp = r"Memória utilizada \(Programação Dinâmica\):\s*(\d+)"
    regex_mem_guloso = r"Memória utilizada \(Guloso\):\s*(\d+)"

    resultados = re.findall(r"Resultado:\s*(-?\d+)\s*Tempo:\s*([0-9.,]+)", output)
    match_mem_dp = re.search(regex_mem_dp, output)
    match_mem_guloso = re.search(regex_mem_guloso, output)

    if len(resultados) < 2:
        return {}

    return {
        "res_dp": resultados[0][0],
        "tempo_dp": resultados[0][1].replace(".", ","),
        "mem_dp": match_mem_dp.group(1) if match_mem_dp else "",
        "res_guloso": resultados[1][0],
        "tempo_guloso": resultados[1][1].replace(".", ","),
        "mem_guloso": match_mem_guloso.group(1) if match_mem_guloso else "",
    }


def main() -> None:
    executable = find_executable()

    if not INSTANCES_DIR.exists():
        raise FileNotFoundError(f"Diretorio de instancias nao encontrado: {INSTANCES_DIR}")

    arquivos = sorted(
        [arquivo for arquivo in INSTANCES_DIR.iterdir() if arquivo.is_file()],
        key=lambda arquivo: extract_instance_key(arquivo.name),
    )

    with OUTPUT_CSV.open("w", encoding="utf-8", newline="") as f_out:
        f_out.write("Entrada;Resultado PD;Tempo PD;Memoria PD (bytes);Resultado Guloso;Tempo Guloso;Memoria Guloso (bytes);\n")

        print("Iniciando bateria de testes...\n")

        for arquivo in arquivos:
            if not re.match(r"^\d+-\d+$", arquivo.name):
                continue

            print(f"Rodando teste para {arquivo.name}...")

            process = subprocess.run(
                [str(executable), str(arquivo)],
                capture_output=True,
                text=True,
                encoding="utf-8",
                errors="replace",
            )

            if process.returncode != 0:
                print(f"[ERRO] O executavel retornou codigo {process.returncode} em {arquivo.name}.")
                if process.stderr:
                    print(process.stderr.strip())
                continue

            dados = parse_output(process.stdout)
            if not dados:
                print(f"[ERRO] Falha ao capturar os dados de {arquivo.name}.")
                print(process.stdout)
                continue

            linha_csv = (
                f"{arquivo.stem};{dados['res_dp']};{dados['tempo_dp']};{dados['mem_dp']};"
                f"{dados['res_guloso']};{dados['tempo_guloso']};{dados['mem_guloso']};\n"
            )
            f_out.write(linha_csv)

    print(f"\nConcluído! Todos os testes foram salvos em:\n{OUTPUT_CSV}")


if __name__ == "__main__":
    main()